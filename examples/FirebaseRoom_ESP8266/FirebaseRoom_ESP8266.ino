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

const int pin_grove = 15;
const int pin_vibrator = 5;
const int pin_light_sensor = A0;
const int pin_led = 12;
const int pin_button = 14;
const int pin_fan = 13;

void setup() {
  Serial.begin(9600);

  pinMode(pin_grove, OUTPUT);
  digitalWrite(pin_grove, HIGH);

  pinMode(pin_vibrator, OUTPUT);
  pinMode(pin_light_sensor, INPUT);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_button, INPUT);
  pinMode(pin_fan, OUTPUT);

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

  Firebase.begin("example.firebaseio.com", "secret_or_token");
}

int button = 0;
float light = 0.0;

void loop() {
  digitalWrite(pin_led, (int)Firebase.get("redlight"));
  digitalWrite(pin_fan, (int)Firebase.get("cooldown"));
  digitalWrite(pin_vibrator, (int)Firebase.get("brrr"));
  int newButton = digitalRead(pin_button);
  if (newButton != button) {
    button = newButton;
    Firebase.set("pushbutton", button);
  }
  float newLight = analogRead(pin_light_sensor);
  if (abs(newLight - light) > 100) {
    light = newLight;
    Firebase.set("sunlight", light);
  }
}
