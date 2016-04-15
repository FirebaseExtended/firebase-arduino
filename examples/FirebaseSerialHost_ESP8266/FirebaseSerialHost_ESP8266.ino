//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


// A sample that will start our serial transciever listening on a software
// port and allow debug over the main serial port.
//
// A suggested setup for testing this example would be a USB to TTL cable
// with the green wire connected to pin 5 and the white wire connected to
// pin 4 (on the huzzah feather esp8266).
// First edit begin.txt and put in your host and secret.
// Then run the following commands to setup the serial port in linux:
// stty -F /dev/ttyUSB0 9600 raw -echo -echoe -echok
// Then on one console do:
// cat /dev/ttyUSB0 &
// This console will now read all responses from the modem. Then do:
// cat begin.txt > /dev/ttyUSB0
// You should see +OK and you can now feed in the other example commmands.

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <Firebase.h>
#include <SerialTransceiver.h>

SoftwareSerial data_serial(5 /*RX*/, 4/*TX*/);
firebase::modem::SerialTransceiver transceiver;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin("SSID", "PASSWORD");
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  data_serial.begin(9600);
  while (!data_serial) {
    Serial.println("Error initilizing serial.");
    delay(5000);
  }

  transceiver.begin(&data_serial);
}

void loop() {
  transceiver.loop();
}
