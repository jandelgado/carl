# Carl music box

[![build firmware](https://github.com/jandelgado/carl/actions/workflows/build.yml/badge.svg)](https://github.com/jandelgado/carl/actions/workflows/build.yml)

This is the firmware repository for the Carl music box. Details will follow
shortly.

<img src=".images/carl.jpg" width=400 alt="carl music box">

<!-- vim-markdown-toc GFM -->

* [Build the firmware](#build-the-firmware)
    * [Arduino IDE](#arduino-ide)
    * [PlatformIO](#platformio)
* [References](#references)
* [Author](#author)

<!-- vim-markdown-toc -->

## Build the firmware

The firmware can be built using the Arduino IDE or PlatformIO.

### Arduino IDE

Check out the repository on your computer. In the Arduino IDE select `Open ...`
under the `File` menu and select the file `carl/carl.ino`.

Select `Arduino Pro or Pro Mini` as the target board under `Tools` > `Board` >
`Arduino AVR Boards`. The processor used is an `Atmega328P 5V 16MHz`.

Install the needed libraries using `Sketch` > `Include Library` > `Manage Libraries...`:
* AnalogMultiButton (1.0.0)
* DFPlayerMini_Fast (1.2.4)
* JLed (4.7.0)
* log4arduino (1.0.0)

Alternatively, install the libraries with this command:
```
arduino --install-library JLed:4.7.0,log4arduino:1.0.0,AnalogMultiButton:1.0.0,DFPlayerMini_Fast:1.2.4
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

* [Carl testdata generator](https://github.com/jandelgado/carl-testdata/)
* [DFRobot Library](https://github.com/DFRobot/DFRobotDFPlayerMini)
* [Makuna Library](https://github.com/Makuna/DFMiniMp3)
* [Powerbroker2 Library](https://github.com/PowerBroker2/DFPlayerMini_Fast)

## Author

(C) Copyright 2021 by Jan Delgado.
