# FirebaseStream

FirebaseStream is a sample that shows basic usage of the `FirebaseArduino`
streaming API.

It displays the current bitcoin price on a OLED screen as it gets updated on
https://publicdata-cryptocurrency.firebaseio.com.

## Hardware setup

1. Get a [Feather Huzzah](https://www.adafruit.com/product/2821)
1. Get a [FeatherWing OLED](https://www.adafruit.com/products/2900)
1. Solder female headers on the `Feather Huzzah`
1. Solder mail headers on the `FeatherWing OLED`
1. Stack the `FeatherWing OLED` on top of the `Feather Huzzah`

## Software setup

1. Install [Arduino 1.6.9](https://www.arduino.cc/en/Main/Software)
1. Install [Arduino ESP8266 core](https://github.com/esp8266/Arduino#installing-with-boards-manager)
1. Download [FirebaseArduino library](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
1. Start Arduino
1. Click `Sketch > Include Library > Add .ZIP Library...`
1. Choose `firebase-arduino-master.zip` downloaded in step `3.`
1. Install the following libraries using `Sketch > Include Library > Manage Libraries...`:
  - [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
  - [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

## Configuration

1. Start Arduino
1. Open `File > Examples > FirebaseArduino > FirebaseStream_ESP8266`
1. Replace `WIFI_SSID` and `WIFI_PASSWORD` with WiFi credentials
1. Select the board `Board > ESP8266 Modules > NodeMCU 1.0`
1. Select the serial port  `Port > /dev/tty...`
1. Select the upload speed `Upload Speed > 115200`
1. Click `Sketch > Upload`

## Play

1. Watch the screen be updated with the current bitcoin price.
1. Watch the data being modified as the sketch run
