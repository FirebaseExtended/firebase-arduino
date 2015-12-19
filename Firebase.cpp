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
#include "Firebase.h"

const char* firebaseFingerprint = "C1 56 CD D8 49 A3 7D D2 1D 49 60 7E 0D 59 A7 7C C1 0E 58 D2";
const int firebasePort = 443;

FirebaseRoot Firebase;

FirebaseRef::FirebaseRef(FirebaseRoot& root, const String& path) : _root(root), _path(path) {
}

FirebaseRef& FirebaseRef::root()  {
  return _root;
}

String FirebaseRef::set(const String& value) {
  return _root.buildRequest("PUT", _path, value);
}

String FirebaseRef::push(const String& value) {
  return _root.buildRequest("POST", _path, value);
}

FirebaseRef FirebaseRef::child(const String& key) {
  return FirebaseRef(_root, _path + "/" + key);
}

FirebaseRoot::FirebaseRoot() : FirebaseRef(*this, "") {
}

void FirebaseRoot::begin(const String& host) {
  _host = host;
}

void FirebaseRoot::auth(const String& token) {
  _token = token;
}

FirebaseRef FirebaseRoot::child(const String& key) {
  return FirebaseRef(*this, key);
}

String FirebaseRoot::buildRequest(const String& method, const String& path, const String& data) {
  String req;
  req += method + " /" + path + ".json";
  if (_token.length() > 0) {
    req += "?auth=" + _token;
  }
  req += " HTTP/1.1\r\n";
  req += "Host: " + _host + "\r\n";
  req += "User-Agent: Arduino\r\n";
  req += "Connection: close\r\n";
  if (data.length()) {
    req += "Content-Length: ";
    req += data.length();
    req += "\r\n\r\n";
    req += data;
  }
  return req;
}

const char* FirebaseRoot::host() const {
  return _host.c_str();
}

uint16_t FirebaseRoot::port() const {
  return firebasePort;
}

const char* FirebaseRoot::fingerprint() const {
  return firebaseFingerprint;
}
