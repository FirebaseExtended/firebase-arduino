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
#include "FirebaseSerial.h"

FirebaseSerialESP8266 FirebaseSerial;

void FirebaseSerialESP8266::begin(const String& host, const String& auth, const String& path) {
  host_ = host;
  auth_ = auth;
  path_ = path;
  stream_.begin(host, auth, path, &httpStream_);
}

bool FirebaseSerialESP8266::connected() {
  return httpStream_.connected();
}

bool FirebaseSerialESP8266::available() {
  return httpStream_.getStreamPtr()->available();
}

String FirebaseSerialESP8266::read() {
  String event;
  auto type = stream_.read(event);
  return event;
}

void FirebaseSerialESP8266::println(String data) {
  push_.begin(host_, auth_, path_, data, &httpPush_);
}
