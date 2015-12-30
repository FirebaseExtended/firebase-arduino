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

// FirebaseError represents a Firebase API error with a code and a
// message.
class FirebaseError {
 public:
  operator bool() const { return _code < 0; }
  int code() const { return _code; }
  const String& message() const { return _message; }
  void reset() { set(0, ""); }
  void set(int code, const String& message) {
    _code = code;
    _message = message;
  }
 private:
  int _code = 0;
  String _message = "";
};

// Firebase is the connection to firebase.
class Firebase {
 public:
  Firebase(const String& host);
  Firebase& auth(const String& auth);
  Firebase& child(const String& key);
  const FirebaseError& error() const {
    return _error;
  }
  String val();
  String push(const String& value);  
 private:
  String sendRequest(const char* method, uint8_t* value = NULL, size_t size = 0);
  HTTPClient _http;
  String _host;
  String _auth;
  String _path;    
  FirebaseError _error;
};


#endif // firebase_h
