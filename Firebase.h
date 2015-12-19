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

// TODO(proppy): add set() method for PUT.
// TODO(proppy): add update() method for PATCH.
// TODO(proppy): add remove() method for DELETE.
// TODO(proppy): add connect() + non blocking
#ifndef firebase_h
#define firebase_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

class FirebaseRoot;

// FirebaseRef is a node in the firebase hierarchy.
//
// Methods val() and push() performs respectivly HTTP GET and POST
// requests on the corresponding child reference and return the
// response body.
class FirebaseRef {
 public:
  FirebaseRef(FirebaseRoot& root, const String& path);
  FirebaseRef& root();
  String val();  
  String push(const String& value);
  FirebaseRef child(const String& key);
 private:
  FirebaseRoot& _root;
  String _path;  
};


// FirebaseError is a string representation of a firebase HTTP error.
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


// FirebaseRoot is the root node of firebase hierarchy.
//
// A global `Firebase` instance is available for convenience, and need
// to be initialized with the `begin()` and `auth()` methods.
class FirebaseRoot : public FirebaseRef {
  friend FirebaseRef;
 public:
  FirebaseRoot();
  FirebaseRoot& begin(const String& host);
  FirebaseRoot& auth(const String& auth);
  FirebaseRef child(const String& key);
  const FirebaseError& error() { return _error; }
 private:
  String sendRequest(const char* method, const String& path, uint8_t* value = NULL, size_t size = 0);
  HTTPClient _http;
  String _host;
  String _auth;
  FirebaseError _error;
};

extern FirebaseRoot Firebase;

#endif // firebase_h
