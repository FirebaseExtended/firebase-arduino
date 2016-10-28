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

#include <Arduino.h>
#include <memory>
#include "FirebaseHttpClient.h"
#include "FirebaseError.h"
#define ARDUINOJSON_USE_ARDUINO_STRING 1
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

class FirebaseGet;
class FirebaseSet;
class FirebasePush;
class FirebaseRemove;
class FirebaseStream;

// Firebase REST API client.
class Firebase {
 public:
  Firebase(const std::string& host, const std::string& auth = "");

  const std::string& auth() const;

  // Fetch json encoded `value` at `path`.
  FirebaseGet get(const std::string& path);
  virtual std::unique_ptr<FirebaseGet> getPtr(const std::string& path);

  // Set json encoded `value` at `path`.
  FirebaseSet set(const std::string& path, const std::string& json);
  virtual std::unique_ptr<FirebaseSet> setPtr(const std::string& path, const std::string& json);

  // Add new json encoded `value` to list at `path`.
  FirebasePush push(const std::string& path, const std::string& json);
  virtual std::unique_ptr<FirebasePush> pushPtr(const std::string& path, const std::string& json);

  // Delete value at `path`.
  FirebaseRemove remove(const std::string& path);
  virtual std::unique_ptr<FirebaseRemove> removePtr(const std::string& path);

  // Start a stream of events that affect value at `path`.
  FirebaseStream stream(const std::string& path);
  virtual std::unique_ptr<FirebaseStream> streamPtr(const std::string& path);

 protected:
  // Used for testing.
  Firebase() {}

 private:
  std::unique_ptr<FirebaseHttpClient> http_;
  std::string host_;
  std::string auth_;
};

class FirebaseCall {
 public:
  FirebaseCall() {}
  FirebaseCall(const std::string& host, const std::string& auth,
               const char* method, const std::string& path,
               const std::string& data = "",
               FirebaseHttpClient* http = NULL);
  virtual ~FirebaseCall();

  virtual const FirebaseError& error() const {
    return error_;
  }

  virtual const std::string& response() const {
    return response_;
  }

  const JsonObject& json();

 protected:
  FirebaseHttpClient* http_;
  FirebaseError error_;
  std::string response_;
  DynamicJsonBuffer buffer_;
};

class FirebaseGet : public FirebaseCall {
 public:
  FirebaseGet() {}
  FirebaseGet(const std::string& host, const std::string& auth,
              const std::string& path, FirebaseHttpClient* http = NULL);

 private:
  std::string json_;
};

class FirebaseSet: public FirebaseCall {
 public:
  FirebaseSet() {}
  FirebaseSet(const std::string& host, const std::string& auth,
              const std::string& path, const std::string& value, FirebaseHttpClient* http = NULL);


 private:
  std::string json_;
};

class FirebasePush : public FirebaseCall {
 public:
  FirebasePush() {}
  FirebasePush(const std::string& host, const std::string& auth,
               const std::string& path, const std::string& value, FirebaseHttpClient* http = NULL);
  virtual ~FirebasePush() {}

  virtual const std::string& name() const {
    return name_;
  }

 private:
  std::string name_;
};

class FirebaseRemove : public FirebaseCall {
 public:
  FirebaseRemove() {}
  FirebaseRemove(const std::string& host, const std::string& auth,
                 const std::string& path, FirebaseHttpClient* http = NULL);
};


class FirebaseStream : public FirebaseCall {
 public:
  FirebaseStream() {}
  FirebaseStream(const std::string& host, const std::string& auth,
                 const std::string& path, FirebaseHttpClient* http = NULL);
  virtual ~FirebaseStream() {}

  // Return if there is any event available to read.
  virtual bool available();

  // Event type.
  enum Event {
    UNKNOWN,
    PUT,
    PATCH
  };

  static inline std::string EventToName(Event event) {
    switch(event)  {
      case UNKNOWN:
        return "UNKNOWN";
      case PUT:
        return "PUT";
      case PATCH:
        return "PATCH";
      default:
        return "INVALID_EVENT_" + event;
    }
  }

  // Read next json encoded `event` from stream.
  virtual Event read(std::string& event);

  const FirebaseError& error() const {
    return _error;
  }

 private:
  FirebaseError _error;
};

#endif // firebase_h
