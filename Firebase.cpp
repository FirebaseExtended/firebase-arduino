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
}  // namespace

Firebase::Firebase(const String& host) : connection_(host) {
}

Firebase& Firebase::auth(const String& auth) {
  connection_.auth(auth);
  return *this;
}

FirebaseResult Firebase::get(const String& path) {
  return connection_.sendRequestGetBody("GET", path);
}

FirebaseResult Firebase::push(const String& path, const String& value) {
  return connection_.sendRequestGetBody("POST", path, value);
}

FirebaseResult Firebase::remove(const String& path) {
  return connection_.sendRequest("DELETE", path);
}

/* FirebaseEventStream */

FirebaseEventStream::FirebaseEventStream(const String& host) : connection_(host) {
}

FirebaseEventStream& FirebaseEventStream::auth(const String& auth) {
  connection_.auth(auth);
  return *this;
}

FirebaseResult FirebaseEventStream::connect(const String& path) {
  String url = connection_.makeURL(path);
  auto& http = connection_.httpClient();
  http.setReuse(true);
  http.begin(connection_.host().c_str(), kFirebasePort, url.c_str(), true,
             kFirebaseFingerprint);
  const char* headers[] = {"Location"};
  http.collectHeaders(headers, 1);
  http.addHeader("Accept", "text/event-stream");
  int statusCode = http.sendRequest("GET", (uint8_t*)NULL, 0);

  String location;
  // TODO(proppy): Add a max redirect check
  while (statusCode == HTTP_CODE_TEMPORARY_REDIRECT) {
    location = http.header("Location");
    http.setReuse(false);
    http.end();
    http.setReuse(true);
    http.begin(location, kFirebaseFingerprint);
    statusCode = http.sendRequest("GET", (uint8_t*)NULL, 0);
  }
  return FirebaseResult(statusCode);
}

bool FirebaseEventStream::connected() {
  return connection_.httpClient().connected();
}

bool FirebaseEventStream::available() {
  return connection_.httpClient().getStreamPtr()->available();
}

FirebaseEventStream::Event FirebaseEventStream::read(String& event) {
  auto client = connection_.httpClient().getStreamPtr();
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

/* FirebaseConnection */

FirebaseConnection::FirebaseConnection(const String& host) : host_(host) {
  http_.setReuse(true);
}

FirebaseConnection& FirebaseConnection::auth(const String& auth) {
  auth_ = auth;
  return *this;
}

FirebaseResult FirebaseConnection::sendRequest(const char* method, const String& path) {
  return sendRequest(method, path, "");
}

FirebaseResult FirebaseConnection::sendRequest(const char* method, const String& path, const String& value) {
  const String url = makeURL(path);
  http_.begin(host_.c_str(), kFirebasePort, url.c_str(), true, kFirebaseFingerprint);
  int statusCode =  http_.sendRequest(method, (uint8_t*)value.c_str(), value.length());
  return FirebaseResult(statusCode);
}

FirebaseResult FirebaseConnection::sendRequestGetBody(const char* method, const String& path) {
  return sendRequestGetBody(method, path, "");
}

FirebaseResult FirebaseConnection::sendRequestGetBody(const char* method, const String& path, const String& value) {
  FirebaseResult result = sendRequest(method, path, value);
  return FirebaseResult(result.httpStatus(), http_.getString());
}

String FirebaseConnection::makeURL(const String& path) {
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  url += path + ".json";
  if (auth_.length() > 0) {
    url += "?auth=" + auth_;
  }
  return url;
}

/* FirebaseResult */

FirebaseResult::FirebaseResult(int status) : status_(status) {
}

FirebaseResult::FirebaseResult(int status, const String& response)
  : status_(status), response_(response) {
}

FirebaseResult::FirebaseResult(const FirebaseResult& other) {
  status_ = other.status_;
  response_ = other.response_;
}

bool FirebaseResult::isOk() const {
  return status_ == HTTP_CODE_OK;
}

bool FirebaseResult::isError() const {
  return status_ < 0;
}

String FirebaseResult::errorMessage() const {
  return HTTPClient::errorToString(status_);
}

const String& FirebaseResult::response() const {
  return response_;
}
