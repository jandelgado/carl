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
#include <Arduino.h>
#include "virtual_keypad.h"  // NOLINT
#include "log.h"

VirtualKeypad::VirtualKeypad(Stream* serial, KeyEventSource* next)
        : serial_(serial), next_(next) {
    help();
}

void VirtualKeypad::help() {
    LOG("[ virtual keypad: press 123456789: playlist, +:next -:prev song");
    LOG("[                 p:play/pause, s:stop, c:config, h: this help");
}

void VirtualKeypad::update() {
    if (serial_->available() > 0) {
        const auto c = serial_->read();
        switch (c) {
            case '1':
                last_ = KeyEvent::kFolder1;
                break;
            case '2':
                last_ = KeyEvent::kFolder2;
                break;
            case '3':
                last_ = KeyEvent::kFolder3;
                break;
            case '4':
                last_ = KeyEvent::kFolder4;
                break;
            case '5':
                last_ = KeyEvent::kFolder5;
                break;
            case '6':
                last_ = KeyEvent::kFolder6;
                break;
            case '7':
                last_ = KeyEvent::kFolder7;
                break;
            case '8':
                last_ = KeyEvent::kFolder8;
                break;
            case '9':
                last_ = KeyEvent::kFolder9;
                break;
            case 's':
                last_ = KeyEvent::kStop;
                break;
            case 'p':
                last_ = KeyEvent::kPlayPause;
                break;
            case '-':
                last_ = KeyEvent::kPrev;
                break;
            case '+':
                last_ = KeyEvent::kNext;
                break;
            case 'c':
                last_ = KeyEvent::kConfigMode;
                break;
            case 'h':
                help();
                // fall through
            default:
                last_ = KeyEvent::kNone;
                break;
        }
    }
    next_->update();
}

