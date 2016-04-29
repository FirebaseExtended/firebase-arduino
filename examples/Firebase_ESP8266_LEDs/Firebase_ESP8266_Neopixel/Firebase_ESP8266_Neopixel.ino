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

// Firebase_ESP8266-Neopixel is a sample that demonstrates how
// to set pixel data using a firebase stream.
#include <Firebase.h>
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>
#include "colors_ext.h"

const int PIN=13;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// TODO: Replace with your own credentials and keep these safe.
Firebase fbase = Firebase("YOUR-PROJECT.firebaseio.com", "YOUR_AUTH_SECRET");

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(25); // 0 ... 255
  strip.show(); // Initialize all pixels to 'off'

  // Not connected, set the LEDs red
  colorWipe(&strip, 0xFF0000, 50);

  // connect to wifi.
  WiFi.begin("GoogleGuest", "");
  Serial.print("connecting");

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    // Draw rainbows while connecting
    Serial.print(".");
    if (count < strip.numPixels()){
       strip.setPixelColor(count++, Wheel(&strip, count * 8));
       strip.show();
    }
    delay(20);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Connected, set the LEDs green
  colorWipe(&strip, 0x00FF00, 50);
}


void loop() {
  // Get all entries.
  // TODO: Replace with streaming
  FirebaseGet get = fbase.get("/rgbdata");
  if (get.error()) {
      Serial.println("Firebase get failed");
      Serial.println(get.error().message());
      return;
  }

  // create an empty object
  const JsonObject& pixelJSON = get.json();

  if(pixelJSON.success()){
    for (int i=0; i < strip.numPixels(); i++) {
      String pixelAddress = "pixel" + String(i);
      String pixelVal = pixelJSON[pixelAddress];
      Serial.println(pixelVal);
      strip.setPixelColor(i, pixelVal.toInt());
    }
    strip.show();
  } else {
    Serial.println("Parse fail.");
    Serial.println(get.response());
  }
}

