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

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

void ConnectWifi(const String& ssid, const String& password = "") {
  if (pass != "") {
    WiFi.begin(ssid, password);
  } else {
    WiFi.begin(ssid);
  }

  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  ConnectWifi(SSID, PASS);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // power grove connector
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  // pin 5 is connected to a vibrator motor.
  pinMode(5, OUTPUT);
  // pin A0 is connected to a light sensor.
  pinMode(A0, INPUT);
  // pin 12 is connected to a red LED.
  pinMode(12, OUTPUT);
  // pin 14 is connected to a push button.
  pinMode(14, INPUT);
  // pin 13 is connected to a fan.
  pinMode(13, OUTPUT);
}

int button = 0;
float light = 0.0;

void loop() {
  digitalWrite(12, (int)Firebase.get("redlight"));
  digitalWrite(13, (int)Firebase.get("cooldown"));
  digitalWrite(5, (int)Firebase.get("brrr"));
  int newButton = digitalRead(14);
  if (newButton != button) {
    button = newButton;
    Firebase.set("pushbutton", button);
  }
  float newLight = analogRead(A0);
  if (abs(newLight - light) > 100) {
    light = newLight;
    Firebase.set("sunlight", light);
  }
}
