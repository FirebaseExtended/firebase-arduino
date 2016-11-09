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

const int grovePowerPin = 15;
const int vibratorPin = 5;
const int lightSensorPin = A0;
const int ledPin = 12;
const int buttonPin = 14;
const int fanPin = 13;

void setup() {
  Serial.begin(9600);

  pinMode(grovePowerPin, OUTPUT);
  digitalWrite(grovePowerPin, HIGH);

  pinMode(vibratorPin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(fanPin, OUTPUT);

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("pushbutton", 0);
  Firebase.set("sunlight", 0);
  Firebase.set("redlight", 0);
  Firebase.set("cooldown", 0);
  Firebase.set("brrr", 0);
}

int button = 0;
float light = 0.0;

void loop() {
  digitalWrite(ledPin, Firebase.getInt("redlight"));
  digitalWrite(fanPin, Firebase.getInt("cooldown"));
  digitalWrite(vibratorPin, Firebase.getInt("brrr"));
  int newButton = digitalRead(buttonPin);
  if (newButton != button) {
    button = newButton;
    Firebase.setInt("pushbutton", button);
  }
  float newLight = analogRead(lightSensorPin);
  if (abs(newLight - light) > 100) {
    light = newLight;
    Firebase.setFloat("sunlight", light);
  }
  delay(200);
}
