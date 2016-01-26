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

Firebase::Firebase(const String& host) {
  connection_.reset(new FirebaseConnection(host));
}

Firebase& Firebase::auth(const String& auth) {
  connection_->auth(auth);
  return *this;
}

FirebaseCall Firebase::get(const String& path) {
  return FirebaseCall("GET", path, ++current_call_id_, connection_.get());
}

FirebaseCall Firebase::push(const String& path, const String& value) {
  return FirebaseCall("POST", path, value, ++current_call_id_, connection_.get());
}

FirebaseCall Firebase::remove(const String& path) {
  return FirebaseCall("DELETE", path, ++current_call_id_, connection_.get());
}
/*
FirebaseEventStream Firebase::stream(const String& path) {
  return FirebaseEventStream(host_, auth_, path);
}*/

/* FirebaseConnection */
FirebaseConnection::FirebaseConnection(const String& host) : host_(host) {
  http_.setReuse(true);
}

FirebaseConnection& FirebaseConnection::auth(const String& auth) {
  auth_ = auth;
  return *this;
}

int FirebaseConnection::sendRequest(const char* method, const String& path, const String& value) {
  const String url = makeUrl(path, auth_);
  http_.begin(host_.c_str(), kFirebasePort, url.c_str(), true, kFirebaseFingerprint);
  int status = http_.sendRequest(method, (uint8_t*)value.c_str(), value.length());
  if (status == HTTP_CODE_OK) {
    remaining_call_buffer_ = http_.getSize();
  }
  return status;
}

String FirebaseConnection::getString() {
  remaining_call_buffer_ = 0;
  return http_.getString();
}

bool FirebaseConnection::isOwner(int call_id) {
  return owning_call_id_ == call_id;
}

void FirebaseConnection::setOwner(int call_id) {
  owning_call_id_ = call_id;
  drainResponseBuffer();
}

void FirebaseConnection::drainResponseBuffer() {
  auto* stream = http_.getStreamPtr();
  Serial.println("Available ");
  Serial.println(stream->available());


  const int buffer_size = 128;
  uint8_t buffer[buffer_size];
  int read = 0;
  int to_read = (buffer_size < remaining_call_buffer_) ? buffer_size : remaining_call_buffer_;
  //TODO(edcoyne) This only reads what is available. Is this sufficient or should we wait?
  while (remaining_call_buffer_ > 0 && (read = stream->read(buffer, to_read) > 0)) {
    Serial.println("Draining ");
    Serial.println(remaining_call_buffer_);
    remaining_call_buffer_ -= read;
    to_read = (buffer_size < remaining_call_buffer_) ? buffer_size : remaining_call_buffer_;
  }
  Serial.println("Done draining ");
  Serial.println(remaining_call_buffer_);
}

/* FirebaseCall */

FirebaseCall::FirebaseCall(const char* method, const String& path, const String& value,
                           int call_id, FirebaseConnection* connection) 
  : connection_(connection), call_id_(call_id) {
  connection_->setOwner(call_id);
  status_ = connection_->sendRequest(method, path, value);
  if (isError()) {
    error_message_ = String(method) + " " + path + ": " + HTTPClient::errorToString(status_);
  }
}

FirebaseCall::FirebaseCall(const char* method, const String& path, int call_id,
                           FirebaseConnection* connection)
    : FirebaseCall(method, path, "", call_id, connection) {}

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
  if (!connection_->isOwner(call_id_)) {
    setErrorNotOwner();
    return "";
  }
  return connection_->getString();
}

void FirebaseCall::setErrorNotOwner() {
  status_ = kStatusNotConnectionOwner;
  error_message_ = "Connection no longer owns connection";
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


