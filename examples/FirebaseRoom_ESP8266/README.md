# FirebaseRoom

FirebaseRoom is a sample that shows basic usage of FirebaseArduino to push
sensor data to Firebase, and trigger actuators from Firebase.

## Software setup

1. Install [Arduino 1.6.9](https://www.arduino.cc/en/Main/Software)
1. Install [Arduino ESP8266 core](https://github.com/esp8266/Arduino#installing-with-boards-manager)
1. Install [Silicon Labs VCP driver](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx) (for MacOSX and Windows)
1. Download [FirebaseArduino library](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
1. Start Arduino
1. Click `Sketch > Include Library > Add .ZIP Library...`
1. Choose `firebase-arduino-master.zip` downloaded in step `3.`

## Hardware setup

1. Get a [Wio Link](http://www.seeedstudio.com/depot/Wio-Link-p-2604.html) board
1. Connect:
  - a [Grove - Vibration Motor](http://www.seeedstudio.com/wiki/Grove_-_Vibration_Motor) on `pin 5`
  - a [Grove - Light Sensor](http://www.seeedstudio.com/wiki/Grove_-_Light_Sensor) on `pin A0`
  - a [Grove - Red LED](http://www.seeedstudio.com/wiki/Grove_-_Red_LED) on `pin 12`
  - a [Grove - Button](http://www.seeedstudio.com/wiki/Grove_-_Button) on `pin 14`
  - a [Grove - Mini Fan](http://www.seeedstudio.com/wiki/Grove_-_Mini_Fan) on `pin 13`

## Configuration

1. Start Arduino
1. Open `File > Examples > FirebaseArduino > FirebaseRoom_ESP8266`
1. In `FirebaseRoom_ESP8266`: Replace `WIFI_SSID` and `WIFI_PASSWORD` with WiFi credentials
1. Go to https://firebase.google.com/console/ and create a new Firebase Project
1. Go to `Database`
1. Copy the `Database hostname` (Database URL without `https://` and trailing `/`)
1. In `FirebaseRoom_ESP8266`: replace `FIREBASE_HOST` with the `Database Hostname`
1. Go to  `⚙ > Project Settings > Database > Database secrets`
1. Click `Firebase Secrets > Show`
1. Copy the `Database Secret`
1. In `FirebaseRoom_ESP8266`:  Replace `FIREBASE_AUTH` with `Database Secret`
1. Select the board `Board > ESP8266 Modules > NodeMCU 1.0`
1. Select the serial port  `Port > /dev/tty...`
1. Select the upload speed `Upload Speed > 115200`
1. Click `Sketch > Upload`

## Play

1. Go to the Firebase console `Data` section
1. Set `redlight` to `1` then `0`
1. Watch the Red LED turn on and off in the room
1. Same for `cooldown` and `brrr`
1. Press the push button in the room
1. Watch the `pushbutton` value change in the Firebase console
1. Put one hand on the light sensor
1. Watch the `sunlight` value change in the Firebase console

## Homeworks

- Build a [Firebase web app](https://www.firebase.com/docs/web/) that interacts with the room.
- Add Grove modules to modules interactions.
- Connect other Grove modules to the room and submit new [PRs](https://github.com/googlesamples/firebase-arduino/pulls)
- Reduce the number of Firebase API calls using `FirebaseObject` or `FirebaseStream`.
- Watch or star the [GitHub repo repo](https://github.com/googlesamples/firebase-arduino)
- Give [feedback](https://gitter.im/googlesamples/firebase-arduino)
- Report [bugs](https://github.com/googlesamples/firebase-arduino/issues/new)
- [Fork](https://github.com/googlesamples/firebase-arduino#fork-destination-box) and [contribute](https://github.com/googlesamples/firebase-arduino/blob/master/CONTRIBUTING.md)
