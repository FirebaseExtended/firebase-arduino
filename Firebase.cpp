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

const char* firebaseFingerprint = "7A 54 06 9B DC 7A 25 B3 86 8D 66 53 48 2C 0B 96 42 C7 B3 0A";
const uint16_t firebasePort = 443;

FirebaseRoot Firebase;

FirebaseRef::FirebaseRef(FirebaseRoot& root, const String& path) : _root(root), _path(path) {
}

FirebaseRef& FirebaseRef::root()  {
  return _root;
}

String FirebaseRef::val() {
  return _root.sendRequest("GET", _path);
}

String FirebaseRef::push(const String& value) {
  return _root.sendRequest("POST", _path, (uint8_t*)value.c_str(), value.length());
}

FirebaseRef FirebaseRef::child(const String& key) {
  return FirebaseRef(_root, _path + "/" + key);
}

FirebaseRoot::FirebaseRoot() : FirebaseRef(*this, "") {
  _http.setReuse(true);
}

FirebaseRoot& FirebaseRoot::begin(const String& host) {
  _host = host;
  return *this;
}

FirebaseRoot& FirebaseRoot::auth(const String& auth) {
  _auth = auth;
  return *this;
}

FirebaseRef FirebaseRoot::child(const String& key) {
  return FirebaseRef(*this, key);
}

String FirebaseRoot::sendRequest(const char* method, const String& path, uint8_t* value, size_t size) {
  _error.reset();
  String url = "/" + path + ".json";
  if (_auth.length() > 0) {
    url += "?auth=" + _auth;
  }
  _http.begin(_host.c_str(), firebasePort, url.c_str(), true, firebaseFingerprint);
  int statusCode = _http.sendRequest(method, value, size);
  if (statusCode < 0) {
    _error.set(statusCode,
	       String(method) + " " + url + ": "
	       + HTTPClient::errorToString(statusCode));
    return "";
  }
  return _http.getString();
  // NOTE: no end() because reuse.
}
