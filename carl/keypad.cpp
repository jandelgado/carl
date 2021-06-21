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
#include "log.h"

// raw key values as recognized by AnalogMultiButton. Values correspond to 
// wiring order of buttons, see documentation.
namespace RawKey {
enum Type : int {
  kPlayPause = 12,
  kNext = 2,
  kPrev = 1,
  kFolder1 = 9,
  kFolder2 = 10,
  kFolder3 = 11,
  kFolder4 = 8,
  kFolder5 = 7,
  kFolder6 = 6,
  kFolder7 = 3,
  kFolder8 = 4,
  kFolder9 = 5,
};
}

constexpr int Keypad::button_values_[];

Keypad::Keypad(int pin)
    : buttons_(pin, sizeof(button_values_) / sizeof(button_values_[0]),
      button_values_, Keypad::kDebounceDurationMs, Keypad::kAnalogResolution) {}

void Keypad::update() { buttons_.update(); }

// check raw key presses on the connected buttons and translate into a KeyEvent.
// if no button is pressed, return KeyEvent::kNone
KeyEvent::Type Keypad::getKeyEvent() {
  if (buttons_.onPress(RawKey::kFolder1))
    return KeyEvent::kFolder1;
  else if (buttons_.onPress(RawKey::kFolder2))
    return KeyEvent::kFolder2;
  else if (buttons_.onPress(RawKey::kFolder3))
    return KeyEvent::kFolder3;
  else if (buttons_.onPress(RawKey::kFolder4))
    return KeyEvent::kFolder4;
  else if (buttons_.onPress(RawKey::kFolder5))
    return KeyEvent::kFolder5;
  else if (buttons_.onPress(RawKey::kFolder6))
    return KeyEvent::kFolder6;
  else if (buttons_.onPress(RawKey::kFolder7))
    return KeyEvent::kFolder7;
  else if (buttons_.onPress(RawKey::kFolder8))
    return KeyEvent::kFolder8;
  else if (buttons_.onPress(RawKey::kFolder9))
    return KeyEvent::kFolder9;
  else if (buttons_.onPressAfter(RawKey::kPlayPause, kDurationLongPressMs))
    return KeyEvent::kStop;
  else if (buttons_.onPressAfter(RawKey::kNext, kDurationLongPressMs)) 
    return KeyEvent::kConfigMode;
  else if (buttons_.onReleaseBefore(RawKey::kPlayPause, kDurationShortPressMs))
    return KeyEvent::kPlayPause;
  else if (buttons_.onReleaseBefore(RawKey::kNext, kDurationShortPressMs))
      return KeyEvent::kNext;
  else if (buttons_.onReleaseBefore(RawKey::kPrev, kDurationShortPressMs))
      return KeyEvent::kPrev;
  else
    return KeyEvent::kNone;
}
