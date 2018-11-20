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

#include "FirebaseObject.h"

// We need to make a copy of data here, even though it may be large.
// It will need to be long lived.
FirebaseObject::FirebaseObject(const char* data) : data_{data} {
  buffer_.reset(new StaticJsonBuffer<FIREBASE_JSONBUFFER_SIZE>);
  json_ = buffer_.get()->parse(&data_[0]);
  // TODO(proppy): find a way to check decoding error, tricky because
  // ArduinoJson doesn't surface error for variant parsing.
  // See: https://github.com/bblanchon/ArduinoJson/issues/279
}

bool FirebaseObject::getBool(const String& path) const {
  JsonVariant variant = getJsonVariant(path);
  if (!variant.is<bool>()) {
    error_ = "failed to convert to bool";
    return 0;
  }
  error_ = "";
  return static_cast<bool>(variant);
}

int FirebaseObject::getInt(const String& path) const {
  JsonVariant variant = getJsonVariant(path);
  if (!variant.is<int>() && !variant.is<float>()) {
    error_ = "failed to convert to number";
    return 0;
  }
  error_ = "";
  return static_cast<int>(variant);
}

float FirebaseObject::getFloat(const String& path) const {
  JsonVariant variant = getJsonVariant(path);
  if (!variant.is<float>() && !variant.is<int>()) {
    error_ = "failed to convert to number";
    return 0;
  }
  error_ = "";
  return static_cast<float>(variant);
}

String FirebaseObject::getString(const String& path) const {
  JsonVariant variant = getJsonVariant(path);
  if (!variant.is<const char *>() || isNullString(path)) {
    error_ = "failed to convert to string";
    return "";
  }
  error_ = "";
  return static_cast<const char*>(variant);
}

bool FirebaseObject::isNullString(const String& path) const {
  JsonVariant variant = getJsonVariant(path);
  return variant.is<const char *>() && variant.asString() == NULL;
}

JsonVariant FirebaseObject::getJsonVariant(const String& path) const {
  String key(path);
  char* start = &key[0];
  char* end = start + key.length();
  // skip first `/`.
  if (*start == '/') {
    start++;
  }
  JsonVariant json = json_;
  while (start < end) {
    // TODO(proppy) split in a separate function.
    char* p = start;
    // advance to next `/`.
    while (*p && (*p != '/')) p++;
    // make `start` a C string.
    *p = 0;
    // return json variant at `start`.
    json = json.asObject().get<JsonVariant>(start);
    // advance to next path element.
    start = p + 1;
  }
  return json;
}

bool FirebaseObject::failed() const {
  return error_.length() > 0;
}

bool FirebaseObject::success() const {
  return error_.length() == 0;
}

const String& FirebaseObject::error() const {
  return error_;
}
