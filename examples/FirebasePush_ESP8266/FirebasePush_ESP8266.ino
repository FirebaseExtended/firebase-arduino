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
Firebase fbase = Firebase("firebase-arduino-example.firebaseio-demo.com")
                     .auth("secret_or_token");

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
  FirebaseObject obj = fbase.create();
  obj[".sv"] = "timestamp";
  FirebaseObject result = fbase.push("/logs", obj);

  // handle error.
  if (result.error()) {
      Serial.print("firebase request failed: ");
      Serial.println(result.error());
      return;
  }
  
  // print result.
  Serial.println(result["name"]);
}

void loop() {
}