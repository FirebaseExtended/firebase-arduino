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

#include <Firebase.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 10
Adafruit_SSD1306 display(OLED_RESET);

Firebase fbase = Firebase("publicdata-cryptocurrency.firebaseio.com");

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();

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
  
  fbase.stream("/bitcoin");
}


void loop() {
  if (fbase.available()) {
    FirebaseObject result = fbase.read();
    if (result.error) {
      Serial.println("firebase streaming error");
      Serial.println(result.error);
      return;
    }
    if (result.json["event"] == "put") {
       String path = result.json["path"];
       float data = result.json["data"];
       Serial.print("path: ");
       Serial.println(path);     
       Serial.print("data: ");
       Serial.println(data);
       if (path != "/_updated") {   
         display.clearDisplay();
         display.setTextSize(2);
         display.setTextColor(WHITE);
         display.setCursor(0,0);
         display.println(path.c_str()+1);
         display.println(data);
         display.display();
       }
     }
  } 
}
