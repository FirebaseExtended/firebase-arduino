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

// FirebaseStream_ESP8266 is a sample that stream on a firebase child
// node.

#include <Firebase.h>

Firebase fbase = Firebase("example.firebaseio.com");

void setup() {
  Serial.begin(9600);

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
  
  fbase.stream("/chat");
}


void loop() {
  if (fbase.error()) {
    Serial.println("streaming error");
    Serial.println(fbase.error().message());
  }
  if (fbase.available()) {
     String event;
     auto type = fbase.read(event);
     Serial.print("event: ");
     Serial.println(type);
     if (type != Firebase::Event::UNKNOWN) {
       Serial.print("data: ");
       Serial.println(event);
     }
  } 
}