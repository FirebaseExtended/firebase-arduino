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


// A sample that will start our serial transciever listening on the primary
// Serial port.
//
// A suggested setup for testing this example would be a esp8266 with built
// in usb connected.
// First edit begin.txt and put in your host and secret.
// Then run the following commands to setup the serial port (assuming ttyUSB0)
// in linux:
// stty -F /dev/ttyUSB0 9600 raw -echo -echoe -echok
// Then on one console do:
// cat /dev/ttyUSB0 &
// This console will now read all responses from the modem. Then do:
// cat begin.txt > /dev/ttyUSB0
// You should see +OK and you can now feed in the other example commmands.

#include <ESP8266WiFi.h>

#include <Firebase.h>
#include <SerialTransceiver.h>

// Set these to run example.
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

firebase::modem::SerialTransceiver transceiver;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  transceiver.RegisterProtocol(new DatabaseProtocol());
  transceiver.begin(&Serial);
}

void loop() {
  transceiver.loop();
}
