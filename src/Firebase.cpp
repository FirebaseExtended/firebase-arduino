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
using std::shared_ptr;

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

void FirebaseCall::analyzeError(char* method, int status, const std::string& path_with_auth) {
    if (status != 200) {
    error_ = FirebaseError(status,
                           std::string(method) + " " + path_with_auth +
                              ": " + http_->errorToString(status));
  } else {
    error_ = FirebaseError();
  }
}

FirebaseCall::~FirebaseCall() {
  http_->end();
}

const JsonObject& FirebaseCall::json() {
  //TODO(edcoyne): This is not efficient, we should do something smarter with
  //the buffers. kotl: Is this still valid?
  if (buffer_.get() == NULL) {
    buffer_.reset(new StaticJsonBuffer<FIREBASE_JSONBUFFER_SIZE>());
  }
  return buffer_.get()->parseObject(response().c_str());
}

// FirebaseRequest
int FirebaseRequest::sendRequest(
  const std::string& host, const std::string& auth,
  char* method, const std::string& path, const std::string& data) {
  std::string path_with_auth = makeFirebaseURL(path, auth);
  http_->setReuseConnection(true);
  http_->begin(host, path_with_auth);
  int status = http_->sendRequest(method, data);
  analyzeError(method, status, path_with_auth);
  response_ = http_->getString();
}

// FirebaseStream
void FirebaseStream::startStreaming(const std::string& host, const std::string& auth, const std::string& path) {
  std::string path_with_auth = makeFirebaseURL(path, auth);
  http_->setReuseConnection(true);
  http_->begin(host, path_with_auth);

  http_->addHeader("Accept", "text/event-stream");
  const char* headers[] = {"Location"};
  http_->collectHeaders(headers, 1);

  int status = http_->sendRequest("GET", "");
  analyzeError("STREAM", status, path_with_auth);

  while (status == HttpStatus::TEMPORARY_REDIRECT) {
      std::string location = http_->header("Location");
      http_->setReuseConnection(false);
      http_->end();
      http_->setReuseConnection(true);
      http_->begin(location);
      status = http_->sendRequest("GET", std::string());
  }
}
