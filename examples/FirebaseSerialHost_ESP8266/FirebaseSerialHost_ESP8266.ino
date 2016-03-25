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

// FirebasePush_ESP8266 is a sample that will start our serial transciever
// listening on a software port and allow debug info over main serial port.

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <Firebase.h>
#include <SerialTransceiver.h>


SoftwareSerial data_serial(13 /*RX*/, 14/*TX*/);
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

  transceiver.begin(&data_serial);
}

void loop() {
  transceiver.loop();
}
