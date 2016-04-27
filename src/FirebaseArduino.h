//
// Copyright 2016 Google Inc.
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

#ifndef FIREBASE_ARDUINO_H
#define FIREBASE_ARDUINO_H

#include "Firebase.h"
#include "FirebaseObject.h"

#ifndef FIREBASE_JSONBUFFER_SIZE
#define FIREBASE_JSONBUFFER_SIZE 200
#endif // FIREBASE_JSONBUFFER_SIZE

class FirebaseArduino {
 public:
  void begin(const char* host, const char* auth = "");
  String push(const String& path, const JsonVariant& value);
  void set(const String& path, const JsonVariant& value);
  FirebaseObject get(const char* path);
  void remove(const char* path);
  void stream(const char* path);
  bool available();
  FirebaseObject readEvent();
  bool success();
  bool failed();
  const String& error();
 private:
  String host_;
  String auth_;
  FirebaseError error_;
  std::unique_ptr<FirebaseHttpClient> http_;
};

extern FirebaseArduino Firebase;

#endif // FIREBASE_ARDUINO_H
