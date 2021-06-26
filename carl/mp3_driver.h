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
#include <stdint.h>

// abstracts the actual mp3 hardware used so that we can use different
// libraries or modules without changing application code.
class Mp3Driver {
 public:
    // start playback
    virtual void start() = 0;
    // pause playback
    virtual void pause() = 0;
    // stop playback
    virtual void stop() = 0;
    // play song from given folder, where files are named ##/###*.mp3
    virtual void playSongFromFolder(uint8_t folder, uint8_t  song) = 0;
    // play song from given large folder, where files are named ##/####*.mp3
    virtual void playSongFromLargeFolder(uint8_t folder, uint16_t song) = 0;
    // return maximum allowed volume
    virtual uint8_t getMaxVolume() const = 0;
    // set volume. volume is in range [0..getMaxVolume()]
    virtual void setVolume(uint8_t vol) = 0;
    // set the equalizer mode. mode is in range [0..getNumEqModes()-1]
    virtual void setEqMode(uint8_t mode) = 0;
    // return numer of equalizer modes
    virtual uint8_t getNumEqModes() const = 0;
    // returns number of files found in the given folder. -1 if none or error
    virtual int16_t getFileCountInFolder(uint8_t folder) = 0;
    // return true if the player is currently playing, else false.
    virtual bool isBusy() = 0;
    // call periodically from the main loop to update the object
    virtual void update() = 0;
};
