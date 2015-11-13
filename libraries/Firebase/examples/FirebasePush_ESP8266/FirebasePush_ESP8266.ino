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

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Firebase.h>

WiFiClientSecure wifiClient;

int pushButtonPin = 1;
int previousState = 1;

void setup() {
    pinMode(pushButtonPin, INPUT);
    digitalWrite(pushButtonPin, HIGH);
 
    Serial.begin(9600);

    WiFi.begin("my-ssid", "my-password");
    Firebase.begin(wifiClient, "my-firebase.firebaseio.com");
    Firebase.auth("my-firebase-token-or-secrent");
}

void loop() {
  int buttonState = digitalRead(pushButtonPin);
  if (buttonState == 0 && buttonState != previousState) {
    String record = Firebase.child("hits").push("{\".sv\": \"timestamp\"}");
    Serial.print(record);
  }
  previousState = buttonState;
}
