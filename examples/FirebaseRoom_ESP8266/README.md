# Firebase Room

## Software setup

1. Install [Arduino 1.6.9](https://www.arduino.cc/en/Main/Software)
1. Install [Arduino ESP8266 core](https://github.com/esp8266/Arduino#installing-with-boards-manager)
1. Download [FirebaseArduino library](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
1. Start Arduino
1. Click `Sketch > Include Library > Add .ZIP Library...`
1. Choose `firebase-arduino-master.zip` downloaded in step 3.

## Hardware setup

1. Get a Wio-link
1. Connect:
- a grove vibrator motor on `pin 5`
- a grove light sensor on `pin A0`
- a grove red LED on `pin 12`
- a grove push button on `pin 14`
- a grove fan on `pin 13`

## Configuration

1. Go to https://www.firebase.com/ and create a new Firebase.
1. Copy the `Firebase Hostname`
1. Go to `Secrets`
1. Click `Firebase Secrets > Shows`
1. Copy the `Firebase Secret`
1. Start Arduino
1. Click `File > Examples > firebase-arduino > FirebaseRoom_ESP8266`
1. Replace `example.firebaseio.com` with the `Firebase Hostname` from step 2.
1. Replace `secret_or_token` with `Firebase Secret` from step 5.
1. Click `Sketch > Upload`

## Play

1. Go to the Firebase console `Data` section
1. Set `redlight` to `1` then `0`
1. Watch the Red LED turn on and off in the room
1. Same for `cooldown` and `brrr`
1. Press the push button in the room
1. Watch the `pushbutton` value change in the Firebase console
1. Put your hand on the light sensor.
1. Watch the `sunlight` value change in the Firebase console

## Homeworks

- Connect other Grove modules to the room and submit new [PRs](https://github.com/googlesamples/firebase-arduino/pulls).
- Reduce the number of Firebase API call using `ArduinoJson`.
- Reduce the number of Firebase API call using `Firebase.stream()`
- Watch or star the [GitHub repo repo](https://github.com/googlesamples/firebase-arduino)
- Give [feedbacks](gitter.im/googlesamples/firebase-arduino)
- Report [bugs](https://github.com/googlesamples/firebase-arduino/issues/new)
- [Fork](https://github.com/googlesamples/firebase-arduino#fork-destination-box) and [contribute](https://github.com/googlesamples/firebase-arduino/blob/master/CONTRIBUTING.md).

