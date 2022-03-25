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

// firebase-arduino is an Arduino client for Firebase.
// It is currently limited to the ESP8266 board family.

#ifndef firebase_h
#define firebase_h

#include "WString.h"
#include <Arduino.h>
#include <memory>
#include <ArduinoJson.h>

#include "FirebaseHttpClient.h"
#include "FirebaseError.h"
#include "FirebaseObject.h"

// Firebase REST API client.
class Firebase {
 public:
  Firebase(const std::string& host, const std::string& auth = "");

  const std::string& auth() const;

  // Fetch json encoded `value` at `path`.
  void get(const std::string& path);

  // Set json encoded `value` at `path`.
  void set(const std::string& path, const std::string& json);

  // Add new json encoded `value` to list at `path`.
  void push(const std::string& path, const std::string& json);

  // Delete value at `path`.
  void remove(const std::string& path);

  // Start a stream of events that affect value at `path`.
  void stream(const std::string& path);

 protected:
  // Used for testing.
  Firebase() {}

 private:
  std::shared_ptr<FirebaseHttpClient> http_;
  std::string host_;
  std::string auth_;
};


class FirebaseCall {
 public:
  FirebaseCall(const std::shared_ptr<FirebaseHttpClient> http = NULL) : http_(http) {}
  virtual ~FirebaseCall();

  const FirebaseError& error() const {
    return error_;
  }

  void analyzeError(char* method, int status, const std::string & path_with_auth);

  const std::string& response() const {
    return response_;
  }

  const JsonObject& json();

 protected:
  const std::shared_ptr<FirebaseHttpClient> http_;
  FirebaseError error_;
  std::string response_;
  std::shared_ptr<StaticJsonBuffer<FIREBASE_JSONBUFFER_SIZE>> buffer_;
};

class FirebaseRequest : public FirebaseCall {
  public:
    FirebaseRequest(const std::shared_ptr<FirebaseHttpClient> http = NULL) : FirebaseCall(http) { }
    virtual ~FirebaseRequest() {}
    int sendRequest(const std::string& host, const std::string& auth,
      char* method, const std::string& path, const std::string& data = "");
};

class FirebaseStream : public FirebaseCall {
 public:
  FirebaseStream(const std::shared_ptr<FirebaseHttpClient> http = NULL) : FirebaseCall(http) { }
  virtual ~FirebaseStream() {}

  void startStreaming(const std::string& host, const std::string& auth, const std::string& path);
};

#endif // firebase_h
