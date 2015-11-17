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

// Firebase is an helper library for building Firebase request.
// TODO(proppy): add value() method for GET.
// TODO(proppy): add update() method for PATCH.
// TODO(proppy): add remove() method for DELETE.
// TODO(proppy): add helper for decoding response.
#ifndef firebase_h
#define firebase_h

#include "Arduino.h"

class Client;
class FirebaseRoot;

// FirebaseRef is a reference in the firebase hierarchy.
//
// Methods `set()` and `push()` returns the HTTP request as a raw
// `String`, the requests then need be sent using an Arduino Wifi,
// Ethernet or GSM `Client`.
class FirebaseRef {
 public:
  FirebaseRef(FirebaseRoot& root, const String& path);
  FirebaseRef& root();
  String set(const String& value);
  String push(const String& value);
  FirebaseRef child(const String& key);
 private:
  FirebaseRoot& _root;
  String _path;  
};

// FirebaseRoot is a root of firebase hierarchy.
//
// A global `Firebase` instance is available for convenience, and need
// to be initialized with the `begin()` and `auth()` methods.
class FirebaseRoot : public FirebaseRef {
  friend FirebaseRef;
 public:
  FirebaseRoot();
  void begin(const String& host);
  void auth(const String& token);
  const char* host() const;
  uint16_t port() const;
  const char* fingerprint() const;
  FirebaseRef child(const String& key);  
 private:
  String buildRequest(const String& method, const String& path, const String& data);
  String _host;
  String _token;
};

extern FirebaseRoot Firebase;

#endif // firebase_h
