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
#include <AnalogMultiButton.h>
#include "key_events.h"

class Keypad {
 public:
    explicit Keypad(int pin);
    void update();

    // return logical key event of last key press or KeyEvent::kNone if no
    // key was pressed
    KeyEvent::Type getKeyEvent();

 private:
    // ** ADD YOUR OWN VALUES HERE / HIER EIGENE WERTE EINTRAGEN **
    //
    // The 12 buttons are connected through one analog input using a resistor
    // network. The actual values depend on the resistor used in the circuit.
    // The values therefore need to be determined by a simple script:
    //    loop(){Serial.println(analogRead(PIN_BUTTONS);}
    //
    // We use a slightly modified circuit, so that current will flow only when a
    // button is pressed (i.e. VCC and GND reversed).  We have to use two tricks
    // to use the AnalogButtons library with our button circuit to work without
    // modifications 1) insert dummy button (100) to catch 0V case  when no
    // button is pressed 2) set analog resoultion to 1100 to detect the 5V
    // (1023) case for button #12.
    //
    static constexpr int button_values_[] = {
        100, /* dummy button 0, will be ignored */
        323, 344, 369, 398, 431, 470, 517, 574, 645, 736, 857, 1023};

    constexpr static uint16_t kAnalogResolution = 1100;
    constexpr static uint16_t kDebounceDurationMs = 20;

    // time [ms] for long/short keypresses.
    constexpr static uint16_t kDurationLongPressMs = 2000;
    constexpr static uint16_t kDurationShortPressMs = 500;

    AnalogMultiButton buttons_;
};
