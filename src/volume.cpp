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
#include "volume.h"  // NOLINT
#include <Arduino.h>

VolumeKnob::VolumeKnob(uint8_t pin_poti, uint8_t pin_power)
    : pin_poti_(pin_poti), pin_power_(pin_power) {
  pinMode(pin_poti, INPUT);
  pinMode(pin_power, OUTPUT);
}

/*
 * read volume potentiometer and return value in range [0..volume_max].
 */
uint16_t VolumeKnob::readVolume(uint16_t volume_max) {
  // only return new reading every SAMPLE_TIME ms
  const auto m = millis();
  if (m - time_last_ < kSampleTimeMs) {
    return last_reading_;
  }
  time_last_ = m;

  // poti gets powered only if needed.
  digitalWrite(pin_power_, HIGH);
  auto value = analogRead(pin_poti_);
  if (value > 1010) {
    value = 1023;
  } else if (value < 10) {
    value = 0;
  }
  digitalWrite(pin_power_, LOW);

  const auto new_reading = volume_max - ((value * volume_max) / 1023);

  // remove jitter on poti readings, see
  // https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
  last_reading_ = (kAlpha * new_reading) + ((1 - kAlpha) * last_reading_);
  return last_reading_;
}
