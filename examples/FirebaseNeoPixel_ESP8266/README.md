# FirebaseNeoPixel

FirebaseNeoPixel is a sample that shows how to set pixel data from Firebase web app.

## Hardware setup

1. Get a [Feather Huzzah](https://www.adafruit.com/product/2821)
1. Get a [FeatherWing NeoPixel](https://www.adafruit.com/product/2945)
1. Cut the default jumper and [solder](https://learn.adafruit.com/adafruit-neopixel-featherwing/pinouts) the one forfor `pin13`
1. Solder female headers on the `Feather Huzzah`
1. Solder mail headers on the `FeatherWing NeoPixel`
1. Stack the `FeatherWing NeoPixel` on top of the `Feather Huzzah`

## Software setup

1. Install [Arduino 1.6.9](https://www.arduino.cc/en/Main/Software)
1. Install [Arduino ESP8266 core](https://github.com/esp8266/Arduino#installing-with-boards-manager)
1. Download [FirebaseArduino library](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
1. Start Arduino
1. Click `Sketch > Include Library > Add .ZIP Library...`
1. Choose `firebase-arduino-master.zip` downloaded in step `3.`
1. Install the following libraries using `Sketch > Include Library > Manage Libraries...`:
  - [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)

## Configuration

1. Go to https://www.firebase.com/ and create a new Firebase
1. Go to `Auth`
1. Scroll to the bottom and click `Add firebase to a web app`
1. Copy the `authDomain`
1. Copy the `apiKey`
1. Start Arduino
1. Open `File > Examples > FirebaseArduino > FirebaseRoom_ESP8266`
1. Replace `WIFI_SSID` and `WIFI_PASSWORD` with WiFi credentials
1. Replace `FIREBASE_HOST` with the `authDomain` from step `4.`
1. Replace `FIREBASE_AUTH` with `apiKey` from step `5.`
1. Select the board `Board > ESP8266 Modules > NodeMCU 1.0`
1. Select the serial port  `Port > /dev/tty...`
1. Select the upload speed `Upload Speed > 115200`
1. Click `Sketch > Upload`
1. Open `public/index.html` with a text editor
1. Replace `example.firebaseio.com` with the `authDomain` from step `4.`
1. Replace `secret_or_token` with `apiKey` from step `5.`
1. Deploy the `public` directory to Firebase hosting
```
npm install -g firebase-tools bower
(cd public && bower install)
firebase login
firebase init
firebase deploy
```

## Play

1. Go to the firebase hosting URL: `firebase-name.firebaseapp.com`
1. Use the color picker and paint the NeoPixel table
1. Watch the NeoPixel being updated to the matching color
