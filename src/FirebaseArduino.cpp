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

// This is needed to compile std::string on esp8266.
template class std::basic_string<char>;

void FirebaseArduino::begin(const String& host, const String& auth) {
  http_.reset(FirebaseHttpClient::create());
  http_->setReuseConnection(true);
  host_ = host.c_str();
  auth_ = auth.c_str();
}

String FirebaseArduino::pushInt(const String& path, int value) {
  return push(path, value);
}

String FirebaseArduino::pushFloat(const String& path, float value) {
  return push(path, value);
}

String FirebaseArduino::pushBool(const String& path, bool value) {
  return push(path, value);
}

String FirebaseArduino::pushString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  return push(path, json);
}

String FirebaseArduino::push(const String& path, const JsonVariant& value) {
  String buf;
  value.printTo(buf);
  auto push = FirebasePush(host_, auth_, path.c_str(), buf.c_str(), http_.get());
  error_ = push.error();
  return push.name().c_str();
}

void FirebaseArduino::setInt(const String& path, int value) {
  set(path, value);
}

void FirebaseArduino::setFloat(const String& path, float value) {
  set(path, value);
}

void FirebaseArduino::setBool(const String& path, bool value) {
  set(path, value);
}

void FirebaseArduino::setString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  set(path, json);
}

void FirebaseArduino::set(const String& path, const JsonVariant& value) {
  String buf;
  value.printTo(buf);
  auto set = FirebaseSet(host_, auth_, path.c_str(), buf.c_str(), http_.get());
  error_ = set.error();
}

FirebaseObject FirebaseArduino::get(const String& path) {
  auto get = FirebaseGet(host_, auth_, path.c_str(), http_.get());
  error_ = get.error();
  if (failed()) {
    return FirebaseObject{""};
  }
  return FirebaseObject(get.response().c_str());
}

int FirebaseArduino::getInt(const String& path) {
  auto get = FirebaseGet(host_, auth_, path.c_str(), http_.get());
  error_ = get.error();
  if (failed()) {
    return 0;
  }
  return FirebaseObject(get.response().c_str()).getInt();
}


float FirebaseArduino::getFloat(const String& path) {
  auto get = FirebaseGet(host_, auth_, path.c_str(), http_.get());
  error_ = get.error();
  if (failed()) {
    return 0.0f;
  }
  return FirebaseObject(get.response().c_str()).getFloat();
}

String FirebaseArduino::getString(const String& path) {
  auto get = FirebaseGet(host_, auth_, path.c_str(), http_.get());
  error_ = get.error();
  if (failed()) {
    return "";
  }
  return FirebaseObject(get.response().c_str()).getString();
}

bool FirebaseArduino::getBool(const String& path) {
  auto get = FirebaseGet(host_, auth_, path.c_str(), http_.get());
  error_ = get.error();
  if (failed()) {
    return "";
  }
  return FirebaseObject(get.response().c_str()).getBool();
}
void FirebaseArduino::remove(const String& path) {
  auto remove = FirebaseRemove(host_, auth_, path.c_str(), http_.get());
  error_ = remove.error();
}

void FirebaseArduino::stream(const String& path) {
  auto stream = FirebaseStream(host_, auth_, path.c_str(), http_.get());
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
  FirebaseObject obj = FirebaseObject(event.c_str());
  obj.getJsonVariant().asObject()["type"] = type;
  return obj;
}

bool FirebaseArduino::success() {
  return error_.code() == 0;
}

bool FirebaseArduino::failed() {
  return error_.code() != 0;
}

const String& FirebaseArduino::error() {
  return error_.message().c_str();
}

FirebaseArduino Firebase;
