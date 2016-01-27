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
#include <ArduinoJson.h>

// FirebaseError is an error for a given firebase API call.
struct FirebaseError {
  FirebaseError() {}
  FirebaseError(int c, const String& m) : code(c), message(m) {}  
  FirebaseError(int c, const char* m) : code(c), message(m) {}
  int code = 0;
  String message = "";
  operator bool() const {
    return code < 0;
  }
};

// FirebaseObject is a payload or a result for a given firebase API call.
class FirebaseObject {
 public:
  FirebaseObject() : _json(_buf.createObject()) {
  }
  FirebaseObject(const FirebaseObject& obj) : FirebaseObject(obj._data) {
  }
  FirebaseObject(const String& data) : _data(data), _json(_buf.parseObject((char*)_data.c_str())) {
    if (!_json.success()) {
      _error = FirebaseError{-1, "error parsing json"};
    }
  }
  FirebaseObject(const FirebaseError& err) : _error{err}, _json(_buf.createObject()) {
  }
  const FirebaseError& error() { return _error; }
  JsonObjectSubscript<const char*> operator[](const char* key) {
    return _json[key];
  }
  size_t printTo(char *buffer, size_t bufferSize) const {
    return _json.printTo(buffer, bufferSize);
  }
 private:
  FirebaseError _error;
  String _data;
  StaticJsonBuffer<200> _buf;
  JsonObject& _json;
};

// Firebase is a client for a given firebase host.
class Firebase {
 public:
  Firebase(const String& host);
  Firebase& auth(const String& auth);
  FirebaseObject get(const String& path);
  FirebaseObject push(const String& path, const FirebaseObject& value);
  bool connected();
  FirebaseObject stream(const String& path);
  bool available();
  FirebaseObject read();
  FirebaseObject create();
 private:
  String makeURL(const String& path);
  FirebaseObject sendRequest(const char* method, const String& path, const String& value = "");
  HTTPClient _http;
  String _host;
  String _auth;
};


#endif // firebase_h
