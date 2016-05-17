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

// FirebaseNeoPixel is a sample that demonstrates how
// to set pixel data from Firebase.
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>

// Set these to run example.
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

const int pixelPin = 13;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(25); // 0 ... 255
  strip.show(); // Initialize all pixels to 'off'

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
}


void loop() {
  // Get all entries.
  // TODO: Replace with streaming
  FirebaseObject pixels = Firebase.get("/rgbdata");
  if (Firebase.failed()) {
      Serial.println("Firebase get failed");
      Serial.println(Firebase.error());
      return;
  }

  for (int i = 0; i < strip.numPixels(); i++) {
    int pixel = pixels.getInt(String("pixel") + i);
    strip.setPixelColor(i, pixel);
  }
  strip.show();
  delay(200);
}
