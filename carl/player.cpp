// Copyright (c) 2021 Jan Delgado <jdelgado[at]gmx.net>
// https://github.com/jandelgado/carl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
#include "player.h"  // NOLINT
#include "log.h"
#include "config.h"

Player::Player(Mp3Module* mp3_module, KeyEventSource* keypad,
               VolumeKnob* volume_knob, JLed* status_led)
    : mp3_module_(mp3_module),
      volume_knob_(volume_knob),
      keypad_(keypad),
      status_led_(status_led),
      state_(eState::PLAY_JINGLE_START),
      keypad_mode_(eKeypadMode::PLAYLIST) {}

void Player::update() {
    switch (state_) {
        case eState::PLAYER: {
            keypad_->update();
            mp3_module_->update();
            status_led_->Update();
            updateVolume();
            checkKeyEvents();
        } break;

        case eState::PLAY_JINGLE_START: {
            auto info =
                mp3_module_->getRandomSongFromFolder(Player::kJingleFolder);
            if (info.found_) {
                LOG("p play jingle start");
                // TODO(jd) store&restore old skip mode
                mp3_module_->setSkipMode(Mp3Module::ePlayMode::SINGLE_SONG);
                mp3_module_->setEventPlaySong(info.folder_, info.song_);
                start_time_jingle_ = millis();
                status_led_->FadeOn(1000).DelayAfter(500).Forever();
                mp3_module_->setVolume(kVolumeJingle);
                state_ = eState::PLAY_JINGLE;
            } else {
                // no jingles found, enter standard player mode
                LOG("p play no jingles found");
                state_ = eState::PLAYER;
            }
        } break;

        case eState::PLAY_JINGLE: {
            // while playing the jingle, controls are intentionally disabled ;)
            status_led_->Update();
            mp3_module_->update();
            if (!mp3_module_->isBusy() &&
                (millis() - start_time_jingle_) > kTimeWaitPlayerToStart) {
                LOG("p play jingle finished");
                mp3_module_->setEventStop();
                // Jingle is finished. Set Breathe effect and song 0/0
                mp3_module_->setSkipMode(Mp3Module::ePlayMode::REPEAT);
                mp3_module_->setNextSong(0, 0);
                status_led_->Breathe(4000).DelayAfter(3000).Forever();
                state_ = eState::PLAYER;
            }
        } break;
    }
}

/*
 * read volume setting from poti and update players volume if there was
 * a change
 */
uint8_t Player::updateVolume() {
    if (millis() - last_volume_update_time_ < kVolumePollDelay) {
        return last_volume_;
    }
    last_volume_update_time_ = millis();
    auto volume = volume_knob_->readVolume(kVolumeMax);

    if (volume != last_volume_) {
        mp3_module_->setVolume(volume);
        last_volume_ = volume;
    }
    return volume;
}

/*
 * check for new key events
 */
void Player::checkKeyEvents() {
    const auto keyEvent = keypad_->getKeyEvent();

    switch (keyEvent) {
        case KeyEvent::kNone:
            break;
        case KeyEvent::kStop:
            LOG("k stop");
            mp3_module_->setEventStop();
            status_led_->Breathe(4000).DelayAfter(6000).Forever();
            break;
        case KeyEvent::kPlayPause:
            LOG("k play/pause");
            if (keypad_mode_ == eKeypadMode::PLAYLIST) {
                if (mp3_module_->isBusy()) {
                    // entering pause
                    status_led_->Breathe(4000).DelayAfter(6000).Forever();
                } else {
                    status_led_->FadeOff(1000).Repeat(1);
                }
                mp3_module_->setEventPlayPause();
            } else {
                // End config mode
                LOG("ending config mode");
                status_led_->Blink(50, 50).Repeat(5);
                keypad_mode_ = eKeypadMode::PLAYLIST;
            }
            break;
        case KeyEvent::kConfigMode:
#ifdef ENABLE_CONFIG_MODE
            LOG("k enter config mode");
            keypad_mode_ = eKeypadMode::CONFIG;
            status_led_->Blink(50, 50).Repeat(5);
#endif
            break;
        case KeyEvent::kNext:
            if (keypad_mode_ == eKeypadMode::PLAYLIST) {
                LOG("k next");
                mp3_module_->setEventNext();
                status_led_->FadeOff(250).Repeat(1);
            }
            break;
        case KeyEvent::kPrev:
            if (keypad_mode_ == eKeypadMode::PLAYLIST) {
                LOG("k prev");
                mp3_module_->setEventPrev();
                status_led_->FadeOff(250).Repeat(1);
            }
            break;
        case KeyEvent::kFolder1:
        case KeyEvent::kFolder2:
        case KeyEvent::kFolder3:
        case KeyEvent::kFolder4:
        case KeyEvent::kFolder5:
        case KeyEvent::kFolder6:
        case KeyEvent::kFolder7:
        case KeyEvent::kFolder8:
        case KeyEvent::kFolder9: {
            const auto folder = keyEvent - KeyEvent::kFolder1;
            LOG("k play folder %d", folder);

            if (keypad_mode_ == eKeypadMode::PLAYLIST) {
                mp3_module_->setEventPlaySong(folder, 0);
                status_led_->FadeOff(250).Repeat(1);
            } else if (keypad_mode_ == eKeypadMode::CONFIG) {
                // in config mode: Folder Button #1 cycles through eq modes
                if (keyEvent == KeyEvent::kFolder1) {
                    mp3_module_->nextEqMode();
                    status_led_->FadeOff(50).Repeat(1);
                }
            }
        } break;
    }
}
