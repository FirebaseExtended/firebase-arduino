# FirebaseArduino

[![Build Status](https://travis-ci.org/firebase/firebase-arduino.svg?branch=master)](https://travis-ci.org/firebase/firebase-arduino)
[![Join the chat at https://gitter.im/googlesamples/firebase-arduino](https://badges.gitter.im/googlesamples/firebase-arduino.svg)](https://gitter.im/googlesamples/firebase-arduino?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Documentation Status](https://readthedocs.org/projects/firebase-arduino/badge/?version=latest)](http://firebase-arduino.readthedocs.io/en/latest/?badge=latest)

This repo contains a collection of samples and an Arduino library that show how to call the [Firebase](https://www.firebase.com/) API from the [ESP8266 Arduino core](https://github.com/esp8266/Arduino).


## Status

![Status: Frozen](https://img.shields.io/badge/Status-Frozen-yellow)

This repository is no longer under active development. No new features will be added and issues are not actively triaged. Pull Requests which fix bugs are welcome and will be reviewed on a best-effort basis.

If you maintain a fork of this repository that you believe is healthier than the official version, we may consider recommending your fork. Please open a Pull Request if you believe that is the case.


## Samples

- [FirebaseDemo](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseDemo_ESP8266) - shows the FirebaseArduino API methods.
- [FirebaseRoom](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseRoom_ESP8266) - shows how to push sensor data and trigger actuator from Firebase.
- [FirebaseStream](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseStream_ESP8266) - shows the FirebaseArduino streaming API.
- [FirebaseNeoPixel](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseNeoPixel_ESP8266) - shows how to control an array of LEDs from a Firebase web app. 

## Documentation

- [FirebaseArduino API Reference](http://firebase-arduino.readthedocs.io/)

## Dependencies
- FirebaseArduino now depends on [ArduinoJson library](https://github.com/bblanchon/ArduinoJson) instead of containing it's own version of it. Please either use Library Manager or download specific version of the library from github. We recommend that ArduinoJson is at least version [5.13.1](https://github.com/bblanchon/ArduinoJson/tree/v5.13.1)

- ESP8266 Core SDK. We recommend using officially tagged releases and it should be at least [2.4.1](https://github.com/esp8266/Arduino/tree/2.4.1)

## Disclaimer

*This is not an official Google product*.
