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

// firebase-arduino is an Arduino client for Firebase.
// It is currently limited to the ESP8266 board family.

#ifndef firebase_cloud_messaging_h
#define firebase_cloud_messaging_h

#include <Arduino.h>
#include <memory>
#include <utility>
#include <vector>
#include "FirebaseHttpClient.h"
#include "FirebaseError.h"
#define ARDUINOJSON_USE_ARDUINO_STRING 1
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

// Defines the actual message to the client, for more detail on
// options and settings see:
// https://firebase.google.com/docs/cloud-messaging/http-server-ref
struct FirebaseCloudMessage {
  // If you set a collapse_key then when the server receives a
  // message, if it already has messages waiting for the client
  // with the same collapse_key it will discard them and deliver
  // this instead.
  std::string collapse_key;

  // If true the user's device will wake to receive the message.
  bool high_priority = false;

  // If true message will not be delivered until device is active.
  bool delay_while_idle = false;

  // Optional, The message will expire after this many seconds.
  // Valid values are 0 to 2,419,200, defaults to max value (4 weeks).
  int time_to_live = -1;

  // Optional, defines the notification to be displayed to the user.
  struct Notification {
    std::string title;
    std::string body;
  };
  Notification notification;

  // Optional, defines data to be delivered to client application.
  std::vector<std::pair<std::string, std::string>> data;

  static FirebaseCloudMessage SimpleNotification(const std::string& title, const std::string& body);
};

// Firebase REST API client.
class FirebaseCloudMessaging {
 public:
  FirebaseCloudMessaging(const std::string& server_key);
  const FirebaseError SendMessageToUser(const std::string& registration_id,
                                        const FirebaseCloudMessage& message);
  const FirebaseError SendMessageToUsers(const std::vector<std::string>& registration_ids,
                                         const FirebaseCloudMessage& message);

  // TODO: The protocol supports sending a message to multiple topics but it is
  // not implemented here.
  const FirebaseError SendMessageToTopic(const std::string& topic,
                                         const FirebaseCloudMessage& message);

 private:
  // Make the actual call to the backend.
  const FirebaseError SendPayload(const char* payload);

  const void AddToJson(const FirebaseCloudMessage& message, JsonObject& json) const;

  std::string auth_header_;
};
#endif  // firebase_cloud_messaging_h
