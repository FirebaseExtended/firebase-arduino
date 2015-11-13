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

#ifndef firebase_h
#define firebase_h

#include "WString.h"

class Client;
class FirebaseRoot;

class FirebaseRef {
 public:
  FirebaseRef(FirebaseRoot& root, const String& path);
  FirebaseRef& root();
  String value();
  String set(const String& key, const String& value);
  String push(const String& value);
  FirebaseRef child(const String& key);
 private:
  FirebaseRoot& _root;
  String _path;  
};

class FirebaseRoot : public FirebaseRef {
  friend FirebaseRef;
 public:
  FirebaseRoot();
  void begin(Client& client, const String& host);
  void auth(const String& token);
  void setError(const String& err);
  const String& error();
 private:
  String makeRequest(const String& method, const String& path, const String& data);
  Client* _client;
  String _host;
  String _token;
  String _err;  
};

extern FirebaseRoot Firebase;

#endif // firebase_h
