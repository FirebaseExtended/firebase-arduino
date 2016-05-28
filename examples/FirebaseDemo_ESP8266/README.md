# FirebaseDemo

FirebaseDemo is a sample that shows basic usage of the `FirebaseArduino` API.

## Software setup

1. Install [Arduino 1.6.9](https://www.arduino.cc/en/Main/Software)
1. Install [Arduino ESP8266 core](https://github.com/esp8266/Arduino#installing-with-boards-manager)
1. Download [FirebaseArduino library](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
1. Start Arduino
1. Click `Sketch > Include Library > Add .ZIP Library...`
1. Choose `firebase-arduino-master.zip` downloaded in step `3.`

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

## Play

1. Go to the Firebase console `Data` section
1. Watch the data being modified as the sketch runs
