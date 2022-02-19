# Carl music box

[![build firmware](https://github.com/jandelgado/carl/actions/workflows/build.yml/badge.svg)](https://github.com/jandelgado/carl/actions/workflows/build.yml)

This is the firmware repository for the Carl music box. More details will
follow shortly.

Carl is featured in the german edition of the Make Magazine in August 2021:

<p float="left">
<img src=".images/make_04_21.jpg" height=300>
<img src=".images/carl.jpg" height=300 alt="carl music box">
</p>

<!-- vim-markdown-toc GFM -->

* [Hardware](#hardware)
    * [Note on DFPlayer Mini Modules](#note-on-dfplayer-mini-modules)
* [Build the firmware](#build-the-firmware)
    * [Configuration](#configuration)
        * [DFPlayer driver library selection](#dfplayer-driver-library-selection)
        * [GD3200B Quirks mode](#gd3200b-quirks-mode)
    * [Arduino IDE](#arduino-ide)
    * [PlatformIO](#platformio)
* [References](#references)
* [Author](#author)

<!-- vim-markdown-toc -->

## Hardware

TODO

### Note on DFPlayer Mini Modules

During the tests of Carl, we encountered different DFPlayer Mini modules, which
turned out to behave differently. DFPlayer's with the `GD3200B` chip for example
were found ~~ **not** to work with any of the available libraries~~ to only work
in a quirks mode.

The differences can easily be spotted and are described below.

| Working                                                 | ~~Not Working~~ Working with Quirks                    |
|---------------------------------------------------------|--------------------------------------------------------|
| 24 pins, labelled `AA20HFJ648-94`                       | 16 pins, labelled `GD3200B`                            |
| <img src=".images/dfplayer_mini_good.jpg" height="200"> | <img src=".images/dfplayer_mini_bad.jpg" height="200"> |

The DFPlayerMini with the `GD3200B` failed reporting correctly the number of
songs per folder. A Quirks mode is provided, to get these modules work with
Carl.

Besides the mentioned `GD3200B` model, there are more models out there which
may be incompatible, see [this site for a testing tool and further
information](https://github.com/ghmartin77/DFPlayerAnalyzer).

## Build the firmware

The firmware can be built using the Arduino IDE or PlatformIO.

### Configuration

The firmware image can be configured either in [config.h](carl/config.h) (for Arduino IDE) or
in [platform.ini](carl/platform.ini). The following options can be configured:

| Option                     | `#define`                    | Default                             |
|----------------------------|------------------------------|-------------------------------------|
| Disable logging            | `NO_LOGGING`                 | unset, i.e. logging is enabled      |
| Enable configuration mode  | `ENABLE_CONFIG_MODE`         | unset, i.e. config mode is disabled |
| Support large folders      | `USE_LARGE_FOLDERS`          | disabled                            |
| Use PowerBroker's driver   | `USE_POWERBROKER_MP3_DRIVER` | this is the default                 |
| Use Makunas's driver       | `USE_MAKUNA_MP3_DRIVER`      |                                     |
| Use DFRobot's driver       | `USE_DFROBOT_MP3_DRIVER`     |                                     |
| Enable GD3200B Quirks Mode | `GD3200B_QUIRKS`             | unset                               |

#### DFPlayer driver library selection

Choose one of the `USE_*_MP3_DRIVER` options. If not set, the PowerBroker
driver will be used. When the firmware is build using platformio (i.e. using
`make`) the required libraries are downloaded automatically.  When using the
Arduino IDE, don't forget to install the actual library used, e.g.:
* [DFPlayerMini_Fast](https://github.com/PowerBroker2/DFPlayerMini_Fast) and [FireTimer](https://github.com/PowerBroker2/FireTimer) for `USE_POWEBROKER_MP3_DRIVER`
* [DFMiniMp3](https://github.com/Makuna/DFMiniMp3) for `USE_MAKUNA_MP3_DRIVER` (recommended for GD3200B models)
* [DFRobotPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini) for `USE_DFROBOT_MP3_DRIVER`

#### GD3200B Quirks mode

When using `GD3200B` based DFPlayer modules, the `GD3200B_QUIRKS` `#define` has
to be set in order to circumvent some incompatibilities (bugs?) these devices
have.  I successfully tested a `GD3200B` DFPlayer using the Makuna-Driver
(`USE_MAKUNA_MP3_DRIVER`) and the quirks mode enabled. Check the log output
to make sure that the `GD3200B` quirks mode is enabled on startup:

```
0(1412): carl starting.
0(1379): using Makuna driver
61(1321): m scanning folders...
62(1319): m enabling GD3200B quirks mode
```

### Arduino IDE

Check out the repository on your computer. In the Arduino IDE select `Open ...`
under the `File` menu and select the file `carl/carl.ino`.

Select `Arduino Pro or Pro Mini` as the target board under `Tools` > `Board` >
`Arduino AVR Boards`. The processor used is an `Atmega328P 5V 16MHz`.

Install the needed libraries using `Sketch` > `Include Library` > `Manage Libraries...`:
* AnalogMultiButton (1.0.0)
* DFPlayerMini_Fast (1.2.4) (Powerbroker DFPlayer Library)
* JLed (4.9.0)
* log4arduino (1.1.0)

Alternatively, install the libraries with this command:
```
arduino --install-library JLed:4.9.0,log4arduino:1.1.0,AnalogMultiButton:1.0.0,DFPlayerMini_Fast:1.2.4,FireTimer:1.0.5
```

Compile and upload the sketch using the corresponding commands in the Arduino
IDE.

### PlatformIO

Check out the repository on your computer. Then `cd carl/carl` and run one of

* `make upload-dfrobot && make monitor`
* `make upload-makuna && make monitor`
* `make upload-powerbroker && make monitor`

to build and upload the firmware and to start the serial monitor. The different
commands correspond to the different drivers for the DFPlayerMini module that
can be used: `DFRobot`, `Makuna` and `PowerBroker2`. Test the different drivers
in case you have problems getting your module running. When building with
the Arduino IDE, the PowerBroker2 driver is used by default.

## References

* [DFRobot DFPlayer Mini]()
* [Carl testdata generator](https://github.com/jandelgado/carl-testdata/)
* [DFRobot DFRobotDFPlayerMini Library](https://github.com/DFRobot/DFRobotDFPlayerMini)
* [Makuna DFMiniMp3 Library](https://github.com/Makuna/DFMiniMp3)
* [Powerbroker2 DFPlayerMini_Fast Library](https://github.com/PowerBroker2/DFPlayerMini_Fast)
* [JLed](https://github.com/jandelgado/jled)
* [log4arduino](https://github.com/jandelgado/log4arduino)
* [DFPlayer Analyzer](https://github.com/ghmartin77/DFPlayerAnalyzer)
* [GD3200 information](https://discourse.voss.earth/t/dfplayer-verschiedene-versionen/681/178)

## Author

(C) Copyright 2021-2022 by Jan Delgado.

