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
#pragma once
#include <Arduino.h>
#include <jled.h>
#include "key_event_source.h"
#include "mp3module.h"
#include "volume.h"

class Player {
 private:
  enum class eState { PLAY_JINGLE, PLAY_JINGLE_START, PLAYER };
  enum class eKeypadMode { PLAYLIST, CONFIG };

  // CONFIG: zero based index of folder containing startup-jingles. Normally
  // first folder after #09 e.g. #10
  static auto constexpr kJingleFolder = 9;

 public:
  Player() = delete;
  Player(Mp3Module* mp3_module, KeyEventSource* keypad, VolumeKnob* volume_knob,
         JLed* status_led);

  // call from main loop() to update state
  void update();

 private:
  // config: maximum allowed volume
  static constexpr uint8_t kVolumeMax = 22;
  // config: volume of jingle
  static constexpr uint8_t kVolumeJingle = 14;
  // time to wait in ms after the player was started before going to check
  // the busy signal
  static constexpr auto kTimeWaitPlayerToStart = 1000;
  // only poll volume know every kVolumePollDelay ms
  static constexpr auto kVolumePollDelay = 150;

  // read & update volume
  uint8_t updateVolume();
  void checkKeyEvents();

  Mp3Module* const mp3_module_;
  VolumeKnob* const volume_knob_;
  KeyEventSource* const keypad_;
  JLed* const status_led_;
  eState state_;
  eKeypadMode keypad_mode_ = eKeypadMode::PLAYLIST;
  uint8_t last_volume_ = 0;
  uint32_t last_volume_update_time_ = 0;
  uint32_t start_time_jingle_;
};
