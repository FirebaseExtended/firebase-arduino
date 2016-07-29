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

// FirebaseCloudMessaging_Send_ESP8266 is a sample that shows sending
// messages to firebase.

#include <ESP8266WiFi.h>

#include <FirebaseCloudMessaging.h>

// Set these to run example.
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define SERVER_KEY "key_from_dashboard"
#define CLIENT_REGISTRATION_ID "key_from_client_after_registration"

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

  FirebaseCloudMessaging fcm(SERVER_KEY);
  FirebaseCloudMessage message =
  	FirebaseCloudMessage::SimpleNotification("Hello World!", "What's happening?");
  FirebaseError error = fcm.SendMessageToUser(CLIENT_REGISTRATION_ID, message);
  if (error) {
    Serial.print("Error:");
    Serial.print(error.code());
    Serial.print(" :: ");
    Serial.println(error.message().c_str());
  } else {
    Serial.println("Sent OK!");
  }
}

void loop() {
}

