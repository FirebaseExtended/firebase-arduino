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
// A suggested setup for testing this example would be to connect a board 
// with integrated usb and open a serial monitor to see debug messages. 
// Then connect another board on pin4 and pin5 to communicate over serial.

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include <Firebase.h>
#include <SerialTransceiver.h>

// Set these to run example.
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

SoftwareSerial data_serial(5 /*RX*/, 4/*TX*/);
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

  data_serial.begin(9600);
  while (!data_serial) {
    Serial.println("Error initilizing serial.");
    delay(5000);
  }

  transceiver.RegisterProtocol(new DatabaseProtocol());
  transceiver.begin(&data_serial);
}

void loop() {
  transceiver.loop();
}
