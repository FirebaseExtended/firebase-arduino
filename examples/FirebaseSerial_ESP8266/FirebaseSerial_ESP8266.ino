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

// FirebaseStream_ESP8266 is a sample that stream bitcoin price from a
// public Firebase and optionally display them on a OLED i2c screen.

#include <FirebaseSerial.h>

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin("SSID", "PASSWORD");
  Serial.print("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  FirebaseSerial.begin("example.firebaseio.com", "secret");
  Serial.print("connecting to firebase");
  while (!FirebaseSerial.connected()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("connected");
}


void loop() {
  if (FirebaseSerial.available()) {
    Serial.println(FirebaseSerial.read());
  }
  delay(1000);
  FirebaseSerial.println("{\".sv\": \"timestamp\"}");
}
