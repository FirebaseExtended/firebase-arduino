# firebase-iot-button

This sample show how to call firebase from an arduino sketch.

*This is not an official Google product*.

## Requirements

- 1 esp8266
- 1 attiny85
- 1 push button
- Arduino 1.6.x

## Setup

- Update `ssid` and `host` variable in `firebase.ino`.
- Flash `firebase.ino` on the esp8266.
- Flash `button.ino` on the attiny85.
- Connect attiny softserial(0,1) on esp8266 rx/tx.

## Provision
- Generate a firebase token.
- Send the following command over serial port on the esp8266:
```
TOKEN <firebase-token>
```

## Run
- Open firebase dashboard on `/hits.json`.
- Press the button.
- Notice the new entry.
