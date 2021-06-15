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
#ifndef VOLUME_H_
#define VOLUME_H_

#include <Arduino.h>

class VolumeKnob {
 public:
  VolumeKnob() = delete;
  explicit VolumeKnob(uint8_t pin_poti, uint8_t pin_power);

  // reads volume and returns value in range [0..volume_max]
  uint16_t readVolume(uint16_t volume_max);

 private:
  static constexpr float kAlpha = 0.6;
  static constexpr auto kSampleTimeMs = 100;
  uint16_t last_reading_ = 0;
  uint8_t pin_poti_;
  uint8_t pin_power_;
  uint32_t time_last_ = 0;
};

#endif  // VOLUME_H_
