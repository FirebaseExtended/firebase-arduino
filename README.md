# FirebaseArduino

[![Build Status](https://travis-ci.org/firebase/firebase-arduino.svg?branch=master)](https://travis-ci.org/firebase/firebase-arduino)
[![Join the chat at https://gitter.im/googlesamples/firebase-arduino](https://badges.gitter.im/googlesamples/firebase-arduino.svg)](https://gitter.im/googlesamples/firebase-arduino?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Documentation Status](https://readthedocs.org/projects/firebase-arduino/badge/?version=latest)](http://firebase-arduino.readthedocs.io/en/latest/?badge=latest)

## Notice, in orer to make it work, please make changes on ESP8266 side
1. libraries\ESP8266HTTPClient\src\ESP8266HTTPClient.cpp
https://github.com/esp8266/Arduino/pull/3438/commits/88a224df101c8ecfa2439915fbb53aff1e9dbfab
2. libraries\ESP8266HTTPClient\src\ESP8266HTTPClient.h
https://github.com/esp8266/Arduino/pull/3439/commits/6d3e12faaef0d8ad70fd26b874bbb86957614c16
3. libraries\ESP8266WiFi\src\WifiClientSecure.cpp
https://github.com/esp8266/Arduino/pull/3440/commits/497c82937687ce8d5137708f146fab7e6890c5e0

This repo contains a collection of samples and an Arduino library that show how to call the [Firebase](https://www.firebase.com/) API from the [ESP8266 Arduino core](https://github.com/esp8266/Arduino).

The Arduino library is [under heavy development](https://github.com/googlesamples/firebase-arduino/issues), **experimental**, **unversioned** and its API is **not stable**.

## Samples

- [FirebaseDemo](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseDemo_ESP8266) - shows the FirebaseArduino API methods.
- [FirebaseRoom](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseRoom_ESP8266) - shows how to push sensor data and trigger actuator from Firebase.
- [FirebaseStream](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseStream_ESP8266) - shows the FirebaseArduino streaming API.
- [FirebaseNeoPixel](https://github.com/googlesamples/firebase-arduino/tree/master/examples/FirebaseNeoPixel_ESP8266) - shows how to control an array of LEDs from a Firebase web app. 

## Documentation

- [FirebaseArduino API Reference](http://firebase-arduino.readthedocs.io/)

## Disclaimer

*This is not an official Google product*.
