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

namespace {
const char* kFirebaseFingerprint = "7A 54 06 9B DC 7A 25 B3 86 8D 66 53 48 2C 0B 96 42 C7 B3 0A";
const uint16_t kFirebasePort = 443;

String makeUrl(const String& path, const String& auth) {
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  url += path + ".json";
  if (auth.length() > 0) {
    url += "?auth=" + auth;
  }
  return url;
}

}  // namespace

Firebase::Firebase(const String& host) : host_(host) {
  http_.setReuse(true);
}

Firebase& Firebase::auth(const String& auth) {
  auth_ = auth;
  return *this;
}

FirebaseCall Firebase::get(const String& path) {
  return FirebaseCall(host_, auth_, "GET", path, &http_);
}

FirebaseCall Firebase::push(const String& path, const String& value) {
  return FirebaseCall(host_, auth_, "POST", path, value, &http_);
}

FirebaseCall Firebase::remove(const String& path) {
  return FirebaseCall(host_, auth_, "DELETE", path, &http_);
}

FirebaseEventStream Firebase::stream(const String& path) {
  return FirebaseEventStream(host_, auth_, path);
}

/* FirebaseCall */

FirebaseCall::FirebaseCall(const String& host, const String& auth,
                           const char* method, const String& path, const String& value,
                           HTTPClient* http) : http_(http) {
  const String url = makeUrl(path, auth);
  http_->begin(host.c_str(), kFirebasePort, url.c_str(), true, kFirebaseFingerprint);
  status_ =  http_->sendRequest(method, (uint8_t*)value.c_str(), value.length());
  if (isError()) {
    error_message_ = String(method) + " " + url + ": " + HTTPClient::errorToString(status_);
  }
}

FirebaseCall::FirebaseCall(const String& host, const String& auth,
                           const char* method, const String& path,
                           HTTPClient* http) : FirebaseCall(host, auth, method, path, "", http) {
}

bool FirebaseCall::isOk() const {
  return status_ == HTTP_CODE_OK;
}

bool FirebaseCall::isError() const {
  return status_ < 0;
}

String FirebaseCall::errorMessage() const {
  return error_message_;
}

String FirebaseCall::rawResponse() {
  return http_->getString();
}

/* FirebaseEventStream */

FirebaseEventStream::FirebaseEventStream(const String& host, const String& auth,
                                         const String& path) {
  const String url = makeUrl(path, auth);
  http_.setReuse(true);
  http_.begin(host.c_str(), kFirebasePort, url.c_str(), true,
              kFirebaseFingerprint);
  const char* headers[] = {"Location"};
  http_.collectHeaders(headers, 1);
  http_.addHeader("Accept", "text/event-stream");
  status_ = http_.sendRequest("GET", (uint8_t*)NULL, 0);

  String location;
  // TODO(proppy): Add a max redirect check
  while (status_ == HTTP_CODE_TEMPORARY_REDIRECT) {
    location = http_.header("Location");
    http_.setReuse(false);
    http_.end();
    http_.setReuse(true);
    http_.begin(location, kFirebaseFingerprint);
    status_ = http_.sendRequest("GET", (uint8_t*)NULL, 0);
  }

  if (status_ != 200) {
    error_message_ = "stream " + location + ": "
       + HTTPClient::errorToString(status_);
  }
}

bool FirebaseEventStream::connected() {
  return http_.connected();
}

bool FirebaseEventStream::available() {
  return http_.getStreamPtr()->available();
}

FirebaseEventStream::Event FirebaseEventStream::read(String& event) {
  auto client = http_.getStreamPtr();
  Event type;
  String typeStr = client->readStringUntil('\n').substring(7);
  if (typeStr == "put") {
    type = FirebaseEventStream::Event::PUT;
  } else if (typeStr == "patch") {
    type = FirebaseEventStream::Event::PATCH;
  } else {
    type = FirebaseEventStream::Event::UNKNOWN;
  }
  event = client->readStringUntil('\n').substring(6);
  client->readStringUntil('\n'); // consume separator
  return type;
}

bool FirebaseEventStream::isError() const {
  return status_ < 0;
}

String FirebaseEventStream::errorMessage() const {
  return error_message_;
}


