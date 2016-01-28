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

class FirebaseGetResult;
class FirebasePushResult;
class FirebaseRemoveResult;
class FirebaseEventStream;

// Primary client to the Firebase backend.
class Firebase {
 public:
  Firebase(const String& host);
  Firebase& auth(const String& auth);

  // Fetch result at "path" to a local variable. If the value is too large you will exceed
  // local memory.
  FirebaseGetResult get(const String& path);

  // Add new value to list at "path", will return child name of new item.
  FirebasePushResult push(const String& path, const String& value);

  // Deletes value at "path" from server.
  FirebaseRemoveResult remove(const String& path);

  // Starts a stream of events that effect object at "path".
  FirebaseEventStream stream(const String& path);

 private:

  int sendRequest(const char* method, const String& path, const String& value);

  HTTPClient http_;
  String host_;
  String auth_;
};


// Result from a Firebase call.
class FirebaseResult {
 public:
  // Constructor for error result.
  FirebaseResult(const String& error_message);
  // Constructor if no error.
  FirebaseResult();

  // True if no error.
  operator bool() const;

  bool isError() const;

  const String& errorMessage() const;

 private:
  bool is_error_ = false;
  String error_message_;
};

class FirebaseRemoveResult : public FirebaseResult {
 public:
  static FirebaseRemoveResult FromError(const String& error_message) {
    return FirebaseRemoveResult(error_message);
  }

  static FirebaseRemoveResult Ok() {
    return FirebaseRemoveResult();
  }

 private:
  FirebaseRemoveResult(const String& error_message);
  FirebaseRemoveResult();
};

class FirebasePushResult : public FirebaseResult {
 public:
  static FirebasePushResult FromError(const String& error_message) {
    return FirebasePushResult(error_message);
  }

  static FirebasePushResult FromResponse(const String& response) {
    FirebasePushResult result;
    // TODO(edcoyne): add json parsing to get name object.
    result.name_ = response;
    return result;
  }

  const String& name() const;

 private:
  FirebasePushResult(const String& error_message);
  FirebasePushResult();

  String name_;
};

class FirebaseGetResult : public FirebaseResult {
 public:
  static FirebaseGetResult FromError(const String& error_message) {
    return FirebaseGetResult(error_message);
  }

  static FirebaseGetResult FromResponse(const String& response) {
    FirebaseGetResult result;
    result.response_ = response;
    return result;
  }

  const String& rawResponse();

 private:
  FirebaseGetResult(const String& error_message);
  FirebaseGetResult();

  String response_;
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

  // True if no error and stream is connected.
  operator bool();

  // True if there was an error.
  bool isError() const;
  String errorMessage() const;

 private:
  HTTPClient http_;
  int status_;
  String error_message_;
};

#endif // firebase_h
