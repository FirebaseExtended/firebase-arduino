//
// Copyright 2016 Google Inc.
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

// FirethingDemo_ESP8266 is a sample that demos operation of the firething
// portion of this library. This is a firmware for the esp that acts as
// a bridge between pins on the esp and a firebase database. This includes
// a captive configuration portal.


#include <ESP8266WiFi.h>
#include <Thing.h>

// No config variables.
// Everything is handled through portal.

thing::FireThing fire_thing;

void setup() {
  Serial.begin(9600);
  Serial.println("Firething starting up...");
  fire_thing.SetDebugHandler([](const char* message) { Serial.println(message); });
  fire_thing.Setup();
}

void loop() {
  fire_thing.Loop();

  Serial.println(".");
  delay(250);
}
