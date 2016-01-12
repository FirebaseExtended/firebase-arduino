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

class FirebaseResult {
 public:
  FirebaseResult(int status);
  FirebaseResult(int status, const String& response);
  FirebaseResult(const FirebaseResult& result);

  bool isError() const;
  bool isOk() const;
  String errorMessage() const;
  const String& response() const;

  int httpStatus() const {
    return status_;
  }

 protected:
  int status_;
  String response_;
};

class FirebaseConnection {
 public:
  FirebaseConnection(const String& host);
  FirebaseConnection& auth(const String& auth);

  const String& host() {
    return host_;
  }

  HTTPClient& httpClient(){
    return http_;
  }

  String makeURL(const String& path);

  FirebaseResult sendRequest(const char* method, const String& path, const String& value);
  FirebaseResult sendRequest(const char* method, const String& path);

  FirebaseResult sendRequestGetBody(const char* method, const String& path);
  FirebaseResult sendRequestGetBody(const char* method, const String& path, const String& value);

 private:
  HTTPClient http_;
  const String host_;
  String auth_;
};

// Firebase is the connection to firebase.
class Firebase {
 public:
  Firebase(const String& host);
  Firebase& auth(const String& auth);

  // Fetch result at "path" to a local variable. If the value is too large you will exceed
  // local memory.
  FirebaseResult get(const String& path);

  // Add new value to list at "path", will return child name of new item.
  FirebaseResult push(const String& path, const String& value);

  // Deletes value at "path" from server.
  FirebaseResult remove(const String& path);

 private:
  FirebaseConnection connection_;
};

// Listens on a stream of events from Firebase backend.
class FirebaseEventStream {
 public:
  enum Event {
    UNKNOWN,
    PUT,
    PATCH
  };

  FirebaseEventStream(const String& host);
  FirebaseEventStream& auth(const String& auth);

  FirebaseResult connect(const String& path);
  Event read(String& event);

  bool connected();
  bool available();
 private:
  FirebaseConnection connection_;
};


#endif // firebase_h
