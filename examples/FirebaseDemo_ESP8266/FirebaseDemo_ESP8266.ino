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

// FirebasePush_ESP8266 is a sample that push a new value to Firebase
// every seconds.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

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
  
  Firebase.begin("example.firebaseio.com", "token_or_secret");
}

void loop() {
  // set value
  String key = "/" + name;
  Firebase.set(key, 1);
  // handle error for last operation
  if (Firebase.failed()) {
      Serial.print("removing /val failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("/val: ");
  Serial.println((int)Firebase.get("/val"));
  delay(1000);

  // remove value
  Firebase.remove("/val");
  delay(1000);

  // push a new value
  String name = Firebase.push("/tmp", 42.0);
  Serial.print("pushed: ");
  Serial.println(name);
  delay(1000);  

  // remove pushed value
  Firebase.remove("/" + name);
  delay(1000);
}
