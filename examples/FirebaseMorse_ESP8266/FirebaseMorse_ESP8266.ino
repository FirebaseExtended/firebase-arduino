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

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Set these to run example.
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"


const int shortMillis = 500;
const int longMillis = shortMillis * 3;

// We define morse . to be binary 0 and - to be binary 1. The longest letter
// is 5 morse elements long so we would have a sparse array of 2^5=32. But
// we need to add a leading 1 to ensure that .- and ..- are not the same value.
// This gives us a size of 2^6=64.
const int morseToCharSize = 64;
char morseToChar[morseToCharSize] = {};

const int oledResetPin = 3;
Adafruit_SSD1306 display(oledResetPin);

const int morseButtonPin = 2;

void updateDisplay(const String& message, const char& indicator, int code);
void initializeMorseToChar();

void setup() {
  initializeMorseToChar();

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
}

String currentMessage;
int currentLetter;
void loop() {
  // Wait while button is up.
  int upStarted = millis();
  while (digitalRead(morseButtonPin) == HIGH) {
    if (millis() - upStarted > longMillis) {      
      updateDisplay(currentMessage, 'w', currentLetter);
    } else if (millis() - upStarted > shortMillis) {
      updateDisplay(currentMessage, 'l', currentLetter);    
    } else {
      updateDisplay(currentMessage, ' ', currentLetter);        
    }
  	delay(1);
  }
  int upTime = millis() - upStarted;
  if (upTime > shortMillis*3) {
    // A letter break is 3 * the length of a short (.).
    if (currentLetter > morseToCharSize || morseToChar[currentLetter] == '\0') {
	    Serial.println("Invalid morse char, ignoring");
    } else {
      currentMessage += morseToChar[currentLetter];
    }
    Serial.println("Listening for new letter.");
    currentLetter = B1;

    // 7 * short timing is a new word.
    if (upTime > shortMillis * 7) {
      currentMessage += " ";
    }
  } // else the upTime < shortMillis we attribute to button bounce.
  Serial.println(currentMessage);
  
  int pressStarted = millis();
  // Wait while button is down.
  while (digitalRead(morseButtonPin) == LOW) {
    if (millis() - pressStarted > longMillis) {      
      updateDisplay(currentMessage, '-', currentLetter);
    } else if (millis() - pressStarted > shortMillis) {
      updateDisplay(currentMessage, '.', currentLetter);    
    } else {
      updateDisplay(currentMessage, ' ', currentLetter);        
    }
  	delay(1);
  }
  int pressTime = millis() - pressStarted;
  if (pressTime > shortMillis) {
    // Shift the binary representation left once then set last bit to 0 or 1.
    currentLetter = (currentLetter << 1) | (pressTime > longMillis);
  }
}

void updateDisplay(const String& message, const char& indicator, int code) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(message);

  display.setTextSize(1);
  display.setCursor(0, display.height() - 10);
  const int mask = 1;
  while (code > 1) {
    display.print((code&mask) ? '-' : '.');
    code = code >> 1;
  }
  display.setCursor(display.width() - 10, display.height() - 10);
  display.print(indicator); 
  display.display();
}

void initializeMorseToChar() {
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
  morseToChar[B110] = 'n';
  morseToChar[B1111] = 'o';
  morseToChar[B10110] = 'p';
  morseToChar[B11101] = 'q';
  morseToChar[B1010] = 'r';
  morseToChar[B1000] = 's';
  morseToChar[B11] = 't';
  morseToChar[B1001] = 'u';
  morseToChar[B10001] = 'v';
  morseToChar[B1011] = 'w';
  morseToChar[B11001] = 'x';
  morseToChar[B11011] = 'y';
  morseToChar[B11100] = 'z';
  morseToChar[B101111] = '1';
  morseToChar[B100111] = '2';
  morseToChar[B100011] = '3';
  morseToChar[B100001] = '4';
  morseToChar[B100000] = '5';
  morseToChar[B110000] = '6';
  morseToChar[B111000] = '7';
  morseToChar[B111100] = '8';
  morseToChar[B111110] = '9';
  morseToChar[B111111] = '0';
}
