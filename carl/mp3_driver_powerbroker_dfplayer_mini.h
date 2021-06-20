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

#ifdef USE_POWERBROKER_MP3_DRIVER

#include <DFPlayerMini_Fast.h>
#include "mp3_driver.h"  // NOLINT
#include <log4arduino.h>

// implementation of the Mp3Driver module for the DFRobot library
// https://github.com/DFRobot/DFRobotDFPlayerMini
class Mp3DriverPowerBrokerDfPlayerMini : public Mp3Driver {
    DFPlayerMini_Fast df_player_;
    uint8_t busy_pin_;
    constexpr auto static kCommTimeoutMs = 500;
    constexpr auto static kDfDebug = false;

 public:
    // The DFPlayer module can be connected to anything satisfying the Stream
    // interface.  Since common base class of serial ports Stream does not
    // have the begin(baud_rate) method, we us a templated ctor here so we call
    // the begin() method of the provided serial port here, regardless of actual
    // type. This way, we keep everythng together and don't forget
    // initialization.
    template <class T>
    Mp3DriverPowerBrokerDfPlayerMini(T* serial, uint8_t busy_pin)
        : busy_pin_(busy_pin) {
        serial->begin(9600);
        pinMode(busy_pin_, INPUT);
        df_player_.begin(*serial, kDfDebug, kCommTimeoutMs);
        df_player_.playbackSource(dfplayer::TF);    // TF = SD Card
    }

    virtual void start() { df_player_.resume(); }

    virtual void pause() { df_player_.pause(); }

    virtual void stop() { df_player_.stop(); }

    virtual void playSongFromFolder(uint8_t folder, uint16_t song) {
        df_player_.playLargeFolder(folder, song);
    }

    virtual void setVolume(uint8_t volume) { df_player_.volume(volume); }

    virtual uint8_t getMaxVolume() const { return 31; }

    virtual void setEqMode(uint8_t mode) { df_player_.EQSelect(mode); }

    virtual uint8_t getNumEqModes() const {
        // DfPlayerMini has 6 different EQ modes
        return 6;
    }

    virtual int16_t getFileCountInFolder(uint8_t folder) {
        return df_player_.numTracksInFolder(folder);
    }

    virtual bool isBusy() { return digitalRead(busy_pin_) == LOW; }

    virtual void update() {
        // nothing to be done here
    }
};

template <class T>
Mp3Driver* new_mp3_driver(T* serial, uint8_t busy_pin) {
    LOG("using PowerBroker driver");
    return new Mp3DriverPowerBrokerDfPlayerMini(serial, busy_pin);
}

#endif
