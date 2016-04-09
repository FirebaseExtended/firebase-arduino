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
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

class FirebaseGet;
class FirebaseSet;
class FirebasePush;
class FirebaseRemove;
class FirebaseStream;

// Firebase REST API client.
class Firebase {
 public:
  Firebase();
  Firebase(const String& host, const String& auth = "");
  void begin(const String& host, const String& auth = "");

  Firebase& host(const String& host);
  Firebase& auth(const String& auth);

  // Fetch json encoded `value` at `path`.
  FirebaseGet get(const String& path);

  // Set json encoded `value` at `path`.
  FirebaseSet set(const String& path, const String& json);

  // Add new json encoded `value` to list at `path`.
  FirebasePush push(const String& path, const String& json);

  // Delete value at `path`.
  FirebaseRemove remove(const String& path);

  // Start a stream of events that affect value at `path`.
  FirebaseStream stream(const String& path);

 private:
  HTTPClient http_;
  String host_;
  String auth_;
};

class FirebaseError {
 public:
  FirebaseError() {}
  FirebaseError(int code, const String& message) : code_(code), message_(message) {
  }
  operator bool() const { return code_ != 0; }
  int code() const { return code_; }
  const String& message() const { return message_; }
 private:
  int code_ = 0;
  String message_ = "";
};

class FirebaseCall {
 public:
  FirebaseCall() {}
  FirebaseCall(const String& host, const String& auth,
               const char* method, const String& path,
               const String& data = "",
               HTTPClient* http = NULL);
  void begin(const String& host, const String& auth,
             const char* method, const String& path,
             const String& data = "",
             HTTPClient* http = NULL);
  const FirebaseError& error() const {
    return error_;
  }
  const String& response() {
    return response_;
  }
 protected:
  HTTPClient* http_;
  FirebaseError error_;
  String response_;
};

class FirebaseGet : public FirebaseCall {
 public:
  FirebaseGet() {}
  FirebaseGet(const String& host, const String& auth,
              const String& path, HTTPClient* http = NULL);

  const String& json() const {
    return json_;
  }

 private:
  String json_;
};

class FirebaseSet: public FirebaseCall {
 public:
  FirebaseSet() {}
  FirebaseSet(const String& host, const String& auth,
	      const String& path, const String& value, HTTPClient* http = NULL);

  const String& json() const {
    return json_;
  }

 private:
  String json_;
};

class FirebasePush : public FirebaseCall {
 public:
  FirebasePush() {}
  FirebasePush(const String& host, const String& auth,
               const String& path, const String& value, HTTPClient* http = NULL);
  void begin(const String& host, const String& auth,
             const String& path, const String& value, HTTPClient* http = NULL);

  const String& name() const {
    return name_;
  }

 private:
  String name_;
};

class FirebaseRemove : public FirebaseCall {
 public:
  FirebaseRemove() {}
  FirebaseRemove(const String& host, const String& auth,
                 const String& path, HTTPClient* http = NULL);
};


class FirebaseStream : public FirebaseCall {
 public:
  FirebaseStream() {}
  FirebaseStream(const String& host, const String& auth,
                 const String& path, HTTPClient* http = NULL);
  void begin(const String& host, const String& auth,
             const String& path, HTTPClient* http = NULL);

  // Return if there is any event available to read.
  bool available();

  // Event type.
  enum Event {
    UNKNOWN,
    PUT,
    PATCH
  };

  // Read next json encoded `event` from stream.
  Event read(String& event);

  const FirebaseError& error() const {
    return _error;
  }

 private:
  FirebaseError _error;
};

#endif // firebase_h
