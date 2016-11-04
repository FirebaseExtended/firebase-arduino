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

// FirebaseDemo_ESP8266 is a sample that binds several pins to paths in
// firebase using the transcriber object.

#include <ESP8266WiFi.h>
#include <Transcriber.h>

// Set these to run example.
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define FIREBASE_PATH "/fthing"
#define DIGITAL_IN D1
#define DIGITAL_OUT BUILTIN_LED
#define ANALOG_IN A0
#define ANALOG_OUT D8
#define ANALOG_ACTIVATION_THRESHOLD 0.1


thing::Config config = {
  FIREBASE_HOST,
  FIREBASE_AUTH,
  FIREBASE_PATH,
  WIFI_SSID,
  WIFI_PASSWORD,
  ANALOG_ACTIVATION_THRESHOLD,
  DIGITAL_IN,
  DIGITAL_OUT,
  ANALOG_IN,
  ANALOG_OUT};

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

  pinMode(DIGITAL_IN, INPUT);
  pinMode(DIGITAL_OUT, OUTPUT);
  pinMode(ANALOG_IN, INPUT);
  pinMode(ANALOG_OUT, OUTPUT);
}

void loop() {
  static thing::Transcriber tscribe(config);
  tscribe.Loop();
  delay(200);
}
