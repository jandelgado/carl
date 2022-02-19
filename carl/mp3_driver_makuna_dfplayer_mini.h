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

#ifdef USE_MAKUNA_MP3_DRIVER

#include <DFMiniMp3.h>
#include "log.h"
#include "mp3_driver.h"  // NOLINT

// implementation of the Mp3Driver module for the Makuna DfMiniMp3 library
// https://github.com/Makuna/DFMiniMp3
class Mp3DriverMakunaDfPlayerMini : public Mp3Driver {
    class Mp3Notify;
    typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
    DfMp3 df_player_;
    uint8_t busy_pin_;

    // implement a notification class, its member methods will get called
    class Mp3Notify {
     public:
        static void PrintlnSourceAction(DfMp3_PlaySources source,
                                        const char* action) {
            if (source & DfMp3_PlaySources_Sd) {
                Serial.print("SD Card, ");
            }
            if (source & DfMp3_PlaySources_Usb) {
                Serial.print("USB Disk, ");
            }
            if (source & DfMp3_PlaySources_Flash) {
                Serial.print("Flash, ");
            }
            Serial.println(action);
            LOG("makuna: source=%d action=%s", source, action);
        }
        static void OnError(uint16_t errorCode) {
            // see DfMp3_Error for code meaning
            Serial.println();
            Serial.print("Com Error ");
            Serial.println(errorCode);
            LOG("makuna: com error %d", errorCode);
        }
        static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {
            Serial.print("Play finished for #");
            Serial.println(track);
            LOG("makuna: play finished for %d", track);
        }
        static void OnPlaySourceOnline(DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "online");
        }
        static void OnPlaySourceInserted(DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "inserted");
        }
        static void OnPlaySourceRemoved(DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "removed");
        }
    };

 public:
    // The DFPlayer module can be connected to anything satisfying the Stream
    // interface, e.g. SoftwareSerial as well as HardwareSerial
    template <class T>
    Mp3DriverMakunaDfPlayerMini(T* serial, uint8_t busy_pin)
        : df_player_(DfMp3(*serial)), busy_pin_(busy_pin) {
        // PIN is low active. By using INPUT_PULLUP isBusy() will return
        // false if the busy_pin GPIO is not correctly connected.
        pinMode(busy_pin_, INPUT_PULLUP);
        df_player_.begin();
        df_player_.setPlaybackSource(DfMp3_PlaySource::DfMp3_PlaySource_Sd);
    }

    void start() override { df_player_.start(); }

    void pause() override { df_player_.pause(); }

    void stop() override { df_player_.stop(); }

    void playSongFromFolder(uint8_t folder, uint8_t song) override {
        df_player_.playFolderTrack(folder, song);
    }

    void playSongFromLargeFolder(uint8_t folder, uint16_t song) override {
        df_player_.playFolderTrack16(folder, song);
    }

    void setVolume(uint8_t volume) override { df_player_.setVolume(volume); }

    uint8_t getMaxVolume() const override { return 30; }

    void setEqMode(uint8_t mode) override {
        df_player_.setEq(static_cast<DfMp3_Eq>(mode));
    }

    uint8_t getNumEqModes() const override {
        // DfPlayerMini has 6 different EQ modes
        return 6;
    }

    int16_t getFileCountInFolder(uint8_t folder) override {
        return df_player_.getFolderTrackCount(folder);
    }

    bool isBusy() override { return digitalRead(busy_pin_) == LOW; }

    void update() override { df_player_.loop(); }

    void reset() override { df_player_.reset(); }
};

template <class T>
Mp3Driver* new_mp3_driver(T* serial, uint8_t busy_pin) {
    LOG("using Makuna driver");
    return new Mp3DriverMakunaDfPlayerMini(serial, busy_pin);
}
#endif
