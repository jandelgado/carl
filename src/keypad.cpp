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
#include "keypad.h"  // NOLINT

constexpr int Keypad::button_values_[];

Keypad::Keypad(int pin)
    : buttons_(pin, sizeof(button_values_) / sizeof(button_values_[0]),
      button_values_, Keypad::kDebounceDurationMs, Keypad::kAnalogResolution) {}

void Keypad::update() { buttons_.update(); }

/*
 * returns the index of pressed playlist button (0..8) or -1 if no playlist
 * button is pressed.
 */
int Keypad::getIndexOfPressedPlaylistButton() {
  // if we ever have more than 9 folders, this will be optimized...
  // check for a playlist folder button press
  if (buttons_.onPress(KeyCode::kFolder1))
    return 0;
  else if (buttons_.onPress(KeyCode::kFolder2))
    return 1;
  else if (buttons_.onPress(KeyCode::kFolder3))
    return 2;
  else if (buttons_.onPress(KeyCode::kFolder4))
    return 3;
  else if (buttons_.onPress(KeyCode::kFolder5))
    return 4;
  else if (buttons_.onPress(KeyCode::kFolder6))
    return 5;
  else if (buttons_.onPress(KeyCode::kFolder7))
    return 6;
  else if (buttons_.onPress(KeyCode::kFolder8))
    return 7;
  else if (buttons_.onPress(KeyCode::kFolder9))
    return 8;
  else
    return -1;
}

bool Keypad::isStopPressed() {
  // PLAY/PAUSE pressed for 2s -> Stop
  return buttons_.onPressAfter(KeyCode::kPlayPause, kDurationLongPressMs);
}

bool Keypad::isEnterConfigModePressed() {
  // NEXT pressed for 2s -> Enter config mode
  return buttons_.onPressAfter(KeyCode::kNext, kDurationLongPressMs);
}

bool Keypad::isPlayPausePressed() {
  return buttons_.onReleaseBefore(KeyCode::kPlayPause, kDurationShortPressMs);
}

bool Keypad::isNextPressed() {
  return buttons_.onReleaseBefore(KeyCode::kNext, kDurationShortPressMs);
}

bool Keypad::isPrevPressed() {
  return buttons_.onReleaseBefore(KeyCode::kPrev, kDurationShortPressMs);
}

bool Keypad::onPressed(KeyCode button) { return buttons_.onPress(button); }
