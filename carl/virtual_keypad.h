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
#include "key_events.h"
#include "key_event_source.h"

// the virtual keypad reads from a serial port and translates charcters
// into key events. The virtual keypad is chained to another KeyEventSource
// which is qeueried when the virtual keyboard has no event.
class VirtualKeypad : public KeyEventSource {
 public:
    VirtualKeypad(Stream* serial, KeyEventSource* next);

    void update() override;

    // return logical key event of last key press or KeyEvent::kNone if no
    // key was pressed
    KeyEvent::Type getKeyEvent() override {
        auto val = last_;
        last_ = KeyEvent::kNone;
        return val != KeyEvent::kNone ? val : next_->getKeyEvent();
    }

 private:
    Stream* serial_;
    KeyEventSource* next_;
    KeyEvent::Type last_;
};
