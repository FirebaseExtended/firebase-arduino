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

// FirebasePush_ESP8266 is a sample that push a new timestamp to firebase
// on each reset.

#include <Firebase.h>

// create firebase client.
Firebase logs = Firebase("example.firebaseio.com")
                   .auth("secret_or_token")
                   .child("child_node");

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

  // add a new entry.
  String l = logs.push("{\".sv\": \"timestamp\"}");
  // handle error.
  if (logs.error()) {
      Serial.println("Firebase request failed");
      Serial.println(logs.error().message());
      return;
  }
  // print response.
  Serial.println(l);
  // print all entries.
  Serial.println(logs.val());
}

void loop() {
}