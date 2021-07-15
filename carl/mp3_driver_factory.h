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
// Instanciate the Mp3Driver to be used. Will either be the Makuna, DFRobot or
// the PowerBroker driver.  Decision is made at compile time by #define'ing
// one of:
//  * USE_MAKUNA_MP3_DRIVER
//  * USE_DFROBOT_MP3_DRIVER
//  * USE_POWERBROKER_MP3_DRIVER
//
#pragma once
#include "mp3_driver.h"
#include "config.h"

#if !defined(USE_MAKUNA_MP3_DRIVER) &&  \
    !defined(USE_DFROBOT_MP3_DRIVER) && \
    !defined(USE_POWERBROKER_MP3_DRIVER)

#warning neither USE_MAKUNA_MP3_DRIVER, USE_POWERBROKER_MP3_DRIVER, \
         USE_DFROBOT_MP3_DRIVER set, defaulting to USE_POWERBROKER_MP3_DRIVER
#define USE_POWERBROKER_MP3_DRIVER
#endif

#if defined(USE_MAKUNA_MP3_DRIVER)
// depends on: "DFPlayer Mini Mp3 by Makuna" (#1561)
#include "mp3_driver_makuna_dfplayer_mini.h"

#elif defined(USE_POWERBROKER_MP3_DRIVER)
// depends on: "DFPlayerMini_Fast" (#6851), "FireTimer" (#7000)
#include "mp3_driver_powerbroker_dfplayer_mini.h"

#elif defined(USE_DFROBOT_MP3_DRIVER)
// depends on: "DFRobotDFPlayerMini" (#1308)
#include "mp3_driver_dfrobot_dfplayer_mini.h"

#else
#error "unknown or none MP3 driver configured."
#endif
