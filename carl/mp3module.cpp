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
#include "mp3module.h"
#include "config.h"

Mp3Module::Mp3Module(Mp3Driver* mp3_driver, ePlayMode skip_mode)
    : mp3_driver_(mp3_driver), skip_mode_(skip_mode) {
#ifdef USE_LARGE_FOLDERS
    LOG("m using large folders");
#endif
    LOG("m scanning folders...");

#ifdef GD3200B_QUIRKS
    LOG("m enabling GD3200B quirks mode");
    // mute since we need to play songs in order to get the file counts below
    mp3_driver_->reset();
    delay(1000);
    setVolume(0);
#endif

    // get number of files per folder so we can later browse the folders.
    memset(folder_count_, 0, sizeof(uint16_t) * kMaxFolders);
    for (auto i = 0; i < kMaxFolders; i++) {
        // some module return -1 even if there are files in the folder.
        // in this case we try to read again
#ifdef GD3200B_QUIRKS
        // this is needed for the GD3200B since otherwise getFileCountInFolder
        // will not return correct values
        unsigned long tmp_time = millis();
        delay(100);
        while (isBusy()) {
        }
        playSongFromFolder(i, 0);
        while (!isBusy() && millis() - tmp_time < 1000) {
        }
        delay(100);
        mp3_driver_->stop();
        tmp_time = millis();
        while (isBusy() && millis() - tmp_time < 1000) {
        }
        delay(100);
#endif

        for (auto curtry = 1; curtry <= kMaxFolderReadTries; curtry++) {
            const auto count = mp3_driver_->getFileCountInFolder(i + 1);
            LOG("  folder %d -> %d songs (%d/%d)", i, count, curtry,
                kMaxFolderReadTries);
            if (count == -1) {
                delay(500);  // workaround for non responding DFPlayers...
                continue;
            }
            folder_count_[i] = count;
            break;
        }
    }

    setEqMode(eq_mode_);
}

// update players internal state, called periodically from main loop
void Mp3Module::update() {
    const auto event = consumeEvent();
    const auto busy = isBusy();

    mp3_driver_->update();

    switch (state_) {
        case eState::PLAYING:
            if (event == eEvent::STOP) {
                LOG("m stop");
                mp3_driver_->stop();
                cur_song_ = 0;
                state_ = eState::STOPPED;
                return;
            }

            if (event == eEvent::PLAY_SONG) {
                LOG("m play song");
                playSongFromFolder(next_folder_, next_song_);
                state_ = eState::START_PLAYING;
                return;
            }

            if (event == eEvent::PLAYPAUSE) {
                LOG("m pause");
                mp3_driver_->pause();
                state_ = eState::PAUSED;
                return;
            }

            if (event == eEvent::PREV) {
                LOG("m prev");
                const auto info = isCurSongPlayingSince(kJumpPrevThresh)
                                      ? getCurSong()
                                      : getPrevSong();
                if (info.found_) {
                    playSongFromFolder(info.folder_, info.song_);
                    state_ = eState::START_PLAYING;
                } else {
                    state_ = eState::STOPPED;
                }
                return;
            }

            if (event == eEvent::NEXT || !busy) {
                if (skip_mode_ == ePlayMode::SINGLE_SONG) {
                    LOG("m play single song > stopped");
                    state_ = eState::STOPPED;
                    return;
                }

                LOG("m next");
                const auto info = getNextSong();
                if (info.found_) {
                    playSongFromFolder(info.folder_, info.song_);
                    state_ = eState::START_PLAYING;
                } else {
                    state_ = eState::STOPPED;
                }
                return;
            }
            break;

        case eState::STOPPED:
            if (event == eEvent::PLAYPAUSE) {
                LOG("m stop > play");
                playSongFromFolder(cur_folder_, cur_song_);
                state_ = eState::START_PLAYING;
                return;
            }

            if (event == eEvent::PLAY_SONG) {
                LOG("m play song");
                playSongFromFolder(next_folder_, next_song_);
                state_ = eState::START_PLAYING;
                return;
            }
            break;

        case eState::PAUSED:
            if (event == eEvent::PLAYPAUSE) {
                LOG("m pause > play");
                mp3_driver_->start();
                state_ = eState::START_PLAYING;
                return;
            }

            if (event == eEvent::PLAY_SONG) {
                LOG("m play song");
                playSongFromFolder(next_folder_, next_song_);
                state_ = eState::START_PLAYING;
                return;
            }
            break;

        case eState::START_PLAYING: {
            time_start_playing_ = millis();
            state_ = eState::WAIT_FOR_PLAYER_TO_START;
        } break;

        case eState::WAIT_FOR_PLAYER_TO_START:
            // after a grace period of 400ms, start watching the busy signal to
            // signal that playback started.
            if (busy &&
                (millis() - time_start_playing_) > kTimeWaitPlayerToStart) {
                // DFPlayer signals that playback has started
                LOG("m playback started ...");
                song_playing_since_ = millis();
                state_ = eState::PLAYING;
            } else {
                if (millis() - time_start_playing_ > kTimeoutStartPlayingMs) {
                    // DFPlayer did not start within expected time, stop.
                    LOG("m playback did not start after %d ms -> stop.",
                        kTimeoutStartPlayingMs);
                    state_ = eState::STOPPED;
                }
            }
            break;
    }
}

// returns true if the player (hardware) is currently playing, otherwise false.
bool Mp3Module::isBusy() const { return mp3_driver_->isBusy(); }

// returns true if the player is playing the current song at least for
// timespan ms
bool Mp3Module::isCurSongPlayingSince(uint32_t timespan) const {
    return song_playing_since_ != 0 &&
           (millis() - song_playing_since_) >= timespan;
}

// set next song to be played on PLAY_SONG event
void Mp3Module::setNextSong(uint8_t folder, uint8_t song) {
    cur_folder_ = folder;
    cur_song_ = song;
}

// get the next available song
Mp3Module::SongInfo Mp3Module::getNextSong() const {
    auto next_song = cur_song_ + 1;
    if (next_song >= folder_count_[cur_folder_]) {
        if (skip_mode_ == ePlayMode::PLAY_ONCE) {
            return SongInfo(false, 0, 0);
        }
        // ePlayMode == REPEAT
        next_song = 0;
    }
    return SongInfo(getNumSongs(cur_folder_) > 0, cur_folder_, next_song);
}

// get the previous available song.
Mp3Module::SongInfo Mp3Module::getPrevSong() const {
    int next_song = cur_song_ - 1;
    if (next_song < 0) {
        if (skip_mode_ == ePlayMode::PLAY_ONCE) {
            return SongInfo(false, 0, 0);
        }
        // ePlayMode == REPEAT
        next_song = max(getNumSongs(cur_folder_) - 1, 0);  // NOLINT
    }
    return SongInfo(getNumSongs(cur_folder_) > 0, cur_folder_, next_song);
}

// start to play given file in folder. count starts with 0 for file and
// folder. Updates cur_song_ and cur_folder_ markers.
void Mp3Module::playSongFromFolder(uint8_t folder, uint16_t song) {
    LOG("play s=%d, f=%d (n=%d)", song, folder, folder_count_[folder]);
    cur_folder_ = folder;
    cur_song_ = song;
#ifdef USE_LARGE_FOLDERS
    mp3_driver_->playSongFromLargeFolder(folder + 1, song + 1);
#else
    mp3_driver_->playSongFromFolder(folder + 1, static_cast<uint8_t>(song + 1));
#endif
}

// Randomly select a song from the given folder
Mp3Module::SongInfo Mp3Module::getRandomSongFromFolder(uint8_t folder) const {
    const auto num_songs = folder_count_[folder];
    if (num_songs == 0) {
        return SongInfo(false, 0, 0);
    } else {
        const auto song = static_cast<int>(random(0, num_songs));
        return SongInfo(true, folder, song);
    }
}

// set the players volume
void Mp3Module::setVolume(uint8_t volume) {
    LOG("m set vol to %d", volume);  // TODO(jd) max volume
    mp3_driver_->setVolume(volume);
    delay(50);  // TODO GD3200B
}

// set the EQ mode
void Mp3Module::setEqMode(uint8_t mode) {
    if (mode > mp3_driver_->getNumEqModes()) return;
    LOG("m set eq to %d", mode);
    eq_mode_ = mode;
    mp3_driver_->setEqMode(eq_mode_);
}

void Mp3Module::nextEqMode() {
    const auto new_mode = (eq_mode_ + 1) % (mp3_driver_->getNumEqModes());
    setEqMode(new_mode);
}
