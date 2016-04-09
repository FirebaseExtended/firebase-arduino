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
#ifndef firebase_serial_h
#define firebase_serial_h

#include "Firebase.h"

class FirebaseSerialESP8266 {
 public:
  FirebaseSerialESP8266() {}
  void begin(const String& host, const String& auth = "", const String& path = "/");
  bool connected();
  bool available();
  String read();
  void println(String data);

 private:
  String host_;
  String auth_;
  String path_;
  HTTPClient httpStream_;
  FirebaseStream stream_;
  HTTPClient httpPush_;
  FirebasePush push_;
};

extern FirebaseSerialESP8266 FirebaseSerial;

#endif // firebase_serial_h
