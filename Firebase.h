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

// firebase-arduino is an Arduino client for Firebase.
// It is currently limited to the ESP8266 board family.

#ifndef firebase_h
#define firebase_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

//TODO(edcoyne) split these into multiple files.

class FirebaseCall {
 public:
  FirebaseCall(const String& host, const String& auth,
               const char* method, const String& path, const String& value,
               HTTPClient* http);
  FirebaseCall(const String& host, const String& auth,
               const char* method, const String& path,
               HTTPClient* http);


  // True if there was an error completing call.
  bool isError() const;
  String errorMessage() const;

  // True if http status code is 200(OK).
  bool isOk() const;

  // Message sent back from Firebase backend. This pulls value to local memory,
  // be careful if value can be large.
  String rawResponse();

  int httpStatus() const {
    return status_;
  }

 private:
  FirebaseCall(HTTPClient* http);

  HTTPClient* http_;

  int status_;
  String error_message_;
};

class FirebaseEventStream {
 public:
  enum Event {
    UNKNOWN,
    PUT,
    PATCH
  };

  FirebaseEventStream(const String& host, const String& auth, const String& path);

  // Read next event in stream.
  Event read(String& event);

  // True if connected to backend.
  bool connected();

  // True if there is an event available.
  bool available();

  // True if there was an error.
  bool isError() const;
  String errorMessage() const;

 private:
  HTTPClient http_;
  int status_;
  String error_message_;
};

// Primary client to the Firebase backend.
class Firebase {
 public:
  Firebase(const String& host);
  Firebase& auth(const String& auth);

  // Fetch result at "path" to a local variable. If the value is too large you will exceed
  // local memory.
  FirebaseCall get(const String& path);

  // Add new value to list at "path", will return child name of new item.
  FirebaseCall push(const String& path, const String& value);

  // Deletes value at "path" from server.
  FirebaseCall remove(const String& path);

  // Starts a stream of events that effect object at "path".
  FirebaseEventStream stream(const String& path);

 private:
  HTTPClient http_;
  String host_;
  String auth_;
};
#endif // firebase_h
