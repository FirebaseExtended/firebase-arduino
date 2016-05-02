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

namespace {
template<typename T>
T decodeJsonLiteral(const String& json) {
  return JsonVariant{ArduinoJson::RawJson{json.c_str()}};
}

// ugly workaround to https://github.com/bblanchon/ArduinoJson/issues/265
template<>
String decodeJsonLiteral<String>(const String& json) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1)> buf;
  String array = "[" + json + "]";
  return buf.parseArray(&array[0])[0];
}
}  // namespace

FirebaseObject::FirebaseObject(const String& data) : data_{data} {
  if (data_[0] == '{') {
    json_ = &buffer_.parseObject(&data_[0]);
  } else if (data_[0] == '"') {
    data_ = decodeJsonLiteral<String>(data_);
  }
}

FirebaseObject::operator bool() {
  return decodeJsonLiteral<bool>(data_);
}

FirebaseObject::operator int() {
    return decodeJsonLiteral<int>(data_);
}

FirebaseObject::operator float() {
    return decodeJsonLiteral<float>(data_);
}

FirebaseObject::operator const String&() {
    return data_;
}

FirebaseObject::operator const JsonObject&() {
  return *json_;
}

JsonObjectSubscript<const char*> FirebaseObject::operator[](const char* key) {
    return json_->operator[](key);
}

JsonObjectSubscript<const String&> FirebaseObject::operator[](const String& key) {
  return json_->operator[](key);
}

JsonVariant FirebaseObject::operator[](JsonObjectKey key) const {
  return json_->operator[](key);
}
