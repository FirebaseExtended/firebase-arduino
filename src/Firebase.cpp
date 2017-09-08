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

using std::unique_ptr;

namespace {
std::string makeFirebaseURL(const std::string& path, const std::string& auth) {
  std::string url;
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

Firebase::Firebase(const std::string& host, const std::string& auth) : host_(host), auth_(auth) {
  http_.reset(FirebaseHttpClient::create());
  http_->setReuseConnection(true);
}

const std::string& Firebase::auth() const {
  return auth_;
}

FirebaseGet Firebase::get(const std::string& path) {
  return FirebaseGet(host_, auth_, path, http_.get());
}

unique_ptr<FirebaseGet> Firebase::getPtr(const std::string& path) {
  return unique_ptr<FirebaseGet>(new FirebaseGet(host_, auth_, path, http_.get()));
}

FirebaseSet Firebase::set(const std::string& path, const std::string& value) {
  return FirebaseSet(host_, auth_, path, value, http_.get());
}

unique_ptr<FirebaseSet> Firebase::setPtr(const std::string& path,
                                         const std::string& value) {
  return unique_ptr<FirebaseSet>(
      new FirebaseSet(host_, auth_, path, value, http_.get()));
}

FirebasePush Firebase::push(const std::string& path, const std::string& value) {
  return FirebasePush(host_, auth_, path, value, http_.get());
}
unique_ptr<FirebasePush> Firebase::pushPtr(const std::string& path, const std::string& value) {
  return unique_ptr<FirebasePush>(
      new FirebasePush(host_, auth_, path, value, http_.get()));
}

FirebaseRemove Firebase::remove(const std::string& path) {
  return FirebaseRemove(host_, auth_, path, http_.get());
}

unique_ptr<FirebaseRemove> Firebase::removePtr(const std::string& path) {
  return unique_ptr<FirebaseRemove>(
      new FirebaseRemove(host_, auth_, path, http_.get()));
}

FirebaseStream Firebase::stream(const std::string& path) {
  // TODO: create new client dedicated to stream.
  return FirebaseStream(host_, auth_, path, http_.get());
}

unique_ptr<FirebaseStream> Firebase::streamPtr(const std::string& path) {
  // TODO: create new client dedicated to stream.
  return unique_ptr<FirebaseStream>(
      new FirebaseStream(host_, auth_, path, http_.get()));
}

// FirebaseCall
FirebaseCall::FirebaseCall(const std::string& host, const std::string& auth,
                           const char* method, const std::string& path,
                           const std::string& data, FirebaseHttpClient* http) : http_(http) {
  std::string path_with_auth = makeFirebaseURL(path, auth);
  http_->setReuseConnection(true);
  http_->begin(host, path_with_auth);

  bool followRedirect = false;
  if (std::string(method) == "STREAM") {
    method = "GET";
    http_->addHeader("Accept", "text/event-stream");
    followRedirect = true;
  }

  if (followRedirect) {
    const char* headers[] = {"Location"};
    http_->collectHeaders(headers, 1);
  }

  int status = http_->sendRequest(method, data);

  // TODO: Add a max redirect check
  if (followRedirect) {
    while (status == HttpStatus::TEMPORARY_REDIRECT) {
      std::string location = http_->header("Location");
      http_->setReuseConnection(false);
      http_->end();
      http_->setReuseConnection(true);
      http_->begin(location);
      status = http_->sendRequest("GET", std::string());
    }
  }

  if (status != 200) {
    error_ = FirebaseError(status,
                           std::string(method) + " " + path_with_auth +
                              ": " + http_->errorToString(status));
  }

  // if not streaming.
  if (!followRedirect) {
    response_ = http_->getString();
  }
}

FirebaseCall::~FirebaseCall() {
  if (http_) {
    http_->end();
  }
}

const JsonObject& FirebaseCall::json() {
  //TODO(edcoyne): This is not efficient, we should do something smarter with
  //the buffers.
  buffer_ = DynamicJsonBuffer();
  return buffer_.parseObject(response().c_str());
}

// FirebaseGet
FirebaseGet::FirebaseGet(const std::string& host, const std::string& auth,
                         const std::string& path,
                         FirebaseHttpClient* http)
  : FirebaseCall(host, auth, "GET", path, "", http) {
}

// FirebaseSet
FirebaseSet::FirebaseSet(const std::string& host, const std::string& auth,
       const std::string& path, const std::string& value,
       FirebaseHttpClient* http)
  : FirebaseCall(host, auth, "PUT", path, value, http) {
  if (!error()) {
    // TODO: parse json
    json_ = response();
  }
}

// FirebasePush
FirebasePush::FirebasePush(const std::string& host, const std::string& auth,
                           const std::string& path, const std::string& value,
                           FirebaseHttpClient* http)
  : FirebaseCall(host, auth, "POST", path, value, http) {
  if (!error()) {
    name_ = json()["name"].as<const char*>();
  }
}

// FirebaseRemove
FirebaseRemove::FirebaseRemove(const std::string& host, const std::string& auth,
                               const std::string& path,
                               FirebaseHttpClient* http)
  : FirebaseCall(host, auth, "DELETE", path, "", http) {
}

// FirebaseStream
FirebaseStream::FirebaseStream(const std::string& host, const std::string& auth,
                               const std::string& path,
                               FirebaseHttpClient* http)
  : FirebaseCall(host, auth, "STREAM", path, "", http) {
}

bool FirebaseStream::available() {
  return http_->getStreamPtr()->available();
}

FirebaseStream::Event FirebaseStream::read(std::string& event) {
  auto client = http_->getStreamPtr();
  Event type;
  std::string typeStr = client->readStringUntil('\n').substring(7).c_str();
  if (typeStr == "put") {
    type = Event::PUT;
  } else if (typeStr == "patch") {
    type = Event::PATCH;
  } else {
    type = Event::UNKNOWN;
  }
  event = client->readStringUntil('\n').substring(6).c_str();
  client->readStringUntil('\n'); // consume separator
  return type;
}
