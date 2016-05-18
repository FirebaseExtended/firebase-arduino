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

// FirebaseStream_ESP8266 is a sample that stream bitcoin price from a
// public Firebase and optionally display them on a OLED i2c screen.

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Set these to run example.
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// We define morse . to be binary 0 and - to be binary 1. The longest letter
// is 5 morse elements long so we would have a sparse array of 2^5=32. But
// we need to add a leading 1 to ensure that .- and ..- are not the same value.
// This gives us a size of 2^6=64.
char morseToChar[64];
morseToChar[B101] = 'a';
morseToChar[B11000] = 'b';
morseToChar[B11010] = 'c';
morseToChar[B1100] = 'd';
morseToChar[B10] = 'e';
morseToChar[B10010] = 'f';
morseToChar[B1110] = 'g';
morseToChar[B10000] = 'h';
morseToChar[B100] = 'i';
morseToChar[B1101] = 'k';
morseToChar[B10100] = 'l';
morseToChar[B111] = 'm';
morseToChar[B1

const int oledResetPin = 3;
Adafruit_SSD1306 display(oledResetPin);

const int morseButtonPin = 2;

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();

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
  
  Firebase.begin("publicdata-cryptocurrency.firebaseio.com");
  Firebase.stream("/bitcoin/last");  
}

const int longPressThresholdMillis = 1000;

bool morseButtonDownTime = 0;
String currentMessage;
String currentMorseLetter;
void loop() {
  // ButtonDown.
  if (morseButtonDownTime == 0 && digitalRead(morseButtonPin) == LOW) {
    morseButtonDownTime = millis();

  // ButtonUp.
  } else if (morseButtonDownTime != 0 && digitalRead(morseButtonPin) == HIGH) {
    const int millisDown = millis() - moreseButtonDownTime;
    morseButtonDownTime = 0;

    const bool longPress = millisDown >= longPressThreshold; 
    currentMorseLetter += longPress ? "." : "_";
  }
}
