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

#include "FirebaseArduino.h"

void FirebaseArduino::begin(const char* host, const char* auth) {
  http_.reset(FirebaseHttpClient::create());
  http_->setReuseConnection(true);
  host_ = host;
  auth_ = auth;
}

String FirebaseArduino::FirebaseArduino::push(const String& path, const JsonVariant& value) {
  String buf;
  value.printTo(buf);
  auto push = FirebasePush(host_, auth_, path, buf, http_.get());
  error_ = push.error();
  return push.name();
}

void FirebaseArduino::set(const String& path, const JsonVariant& value) {
  String buf;
  value.printTo(buf);
  auto set = FirebaseSet(host_, auth_, path, buf, http_.get());
  error_ = set.error();
}

FirebaseObject FirebaseArduino::get(const char* path) {
  auto get = FirebaseGet(host_, auth_, path, http_.get());
  error_ = get.error();
  if (failed()) {
    return FirebaseObject{""};
  }
  return FirebaseObject(get.response());
}

void FirebaseArduino::remove(const char* path) {
  auto remove = FirebaseRemove(host_, auth_, path, http_.get());
  error_ = remove.error();
}

void FirebaseArduino::stream(const char* path) {
  auto stream = FirebaseStream(host_, auth_, path, http_.get());
  error_ = stream.error();
}

bool FirebaseArduino::available() {
  return http_->getStreamPtr()->available();
}

FirebaseObject FirebaseArduino::readEvent() {
  auto client = http_->getStreamPtr();
  String type = client->readStringUntil('\n').substring(7);;
  String event = client->readStringUntil('\n').substring(6);
  client->readStringUntil('\n'); // consume separator
  FirebaseObject obj = FirebaseObject(event);
  obj["type"] = type;
  return obj;
}

bool FirebaseArduino::success() {
  return error_.code() == 0;
}

bool FirebaseArduino::failed() {
  return error_.code() != 0;
}

const String& FirebaseArduino::error() {
  return error_.message();
}

FirebaseArduino Firebase;
