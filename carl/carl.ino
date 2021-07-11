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
#include <AnalogMultiButton.h>
#include <SoftwareSerial.h>
#include <jled.h>

#include "config.h"
#include "keypad.h"
#include "log.h"
#include "mp3_driver.h"
#include "mp3_driver_factory.h"
#include "mp3module.h"
#include "player.h"
#include "virtual_keypad.h"
#include "volume.h"

Player* player_;

/*
 * startup: wire the components of the player
 */
void setup() {
    Serial.begin(9600);
    LOG_INIT(&Serial);
    LOG("carl starting.");

    // DFPlayerMini module and Arduino communicate through software serial iface
    auto mp3serial = new SoftwareSerial(PIN_RX, PIN_TX);
    auto mp3driver = new_mp3_driver(mp3serial, PIN_DFPLAYER_BUSY);
    auto mp3module = new Mp3Module(mp3driver, Mp3Module::ePlayMode::REPEAT);
    auto status_led = new JLed(PIN_LED);
    status_led->LowActive();
    auto keypad = new Keypad(PIN_BUTTONS);
    auto virtual_keypad = new VirtualKeypad(&Serial, keypad);
    auto volume_knob = new VolumeKnob(PIN_VOLUME, PIN_VOLUME_POWER);
    player_ = new Player(mp3module, virtual_keypad, volume_knob, status_led);

    for (auto i = 0; i < 100; i++) {
        randomSeed(analogRead(0));
    }
}

void loop() {
    player_->update();
}
