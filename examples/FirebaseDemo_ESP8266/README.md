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
1. Copy the `Firebase Hostname`
1. Go to `Secrets`
1. Click `Firebase Secrets > Shows`
1. Copy the `Firebase Secret`
1. Start Arduino
1. Click `File > Examples > firebase-arduino > FirebaseRoom_ESP8266`
1. Replace `SSID` and `PASSWORD` with WiFi credentials
1. Replace `example.firebaseio.com` with the `Firebase Hostname` from step `2.`
1. Replace `secret_or_token` with `Firebase Secret` from step `5.`
1. Click `Sketch > Upload`

## Play

1. Go to the Firebase console `Data` section
1. Watch the data being modified as the sketch run.
