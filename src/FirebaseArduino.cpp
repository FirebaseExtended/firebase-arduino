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

#include "FirebaseArduino.h"

const char* kApplicationType = "application/json";
const uint16_t kFirebasePort = 443;
const int kStatusOK = 200;
const int kStatusTemporaryRedirect = 307;

String makeFirebaseURI(const String& path, const String& auth) {
   String uri;
   if (path[0] != '/') {
     uri = "/";
   }
   uri += path;
   uri += ".json";
   if (auth.length() > 0) {
     uri += "?auth=";
     uri += auth;
   }
   return uri;
}

FirebaseArduino::FirebaseArduino(Client& client) : client_(client) {}

void FirebaseArduino::begin(const String& host, const String& auth) {
  host_ = host.c_str();
  auth_ = auth.c_str();
}

void FirebaseArduino::initStream() {
  if (stream_http_.get() == nullptr) {
    stream_http_.reset(new HttpClient(client_, host_, kFirebasePort));
    stream_http_->connectionKeepAlive();
  }
}

void FirebaseArduino::initRequest() {
  if (req_http_.get() == nullptr) {
    req_http_.reset(new HttpClient(client_, host_, kFirebasePort));
    req_http_->connectionKeepAlive();
  }
}

String FirebaseArduino::pushInt(const String& path, int value) {
  return push(path, value);
}

String FirebaseArduino::pushFloat(const String& path, float value) {
  return push(path, value);
}

String FirebaseArduino::pushBool(const String& path, bool value) {
  return push(path, value);
}

String FirebaseArduino::pushString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  return push(path, json);
}

String FirebaseArduino::push(const String& path, const JsonVariant& value) {
  int size = value.measureLength()+1;
  char * buf = new char[size];
  value.printTo(buf, size);
  initRequest();
  String uri = makeFirebaseURI(path, auth_);
  int err = req_http_->post(uri.c_str(), kApplicationType, buf);
  if (err != 0) {
    error_ = FirebaseError(err, "HTTP request failed");
    return "";
  }
  int statusCode = req_http_->responseStatusCode();
  if (statusCode != kStatusOK) {
    error_ = FirebaseError(statusCode, "PUT request failed");
    return "";
  }
  delete buf;
  StaticJsonBuffer<FIREBASE_JSONBUFFER_SIZE> jsonBuffer;
  return jsonBuffer.parseObject(req_http_->responseBody())["name"];
}

void FirebaseArduino::setInt(const String& path, int value) {
  set(path, value);
}

void FirebaseArduino::setFloat(const String& path, float value) {
  set(path, value);
}

void FirebaseArduino::setBool(const String& path, bool value) {
  set(path, value);
}

void FirebaseArduino::setString(const String& path, const String& value) {
  JsonVariant json(value.c_str());
  set(path, json);
}

void FirebaseArduino::set(const String& path, const JsonVariant& value) {
  int size = value.measureLength()+1;
  char* buf= new char[size];
  value.printTo(buf, size);
  initRequest();
  String uri = makeFirebaseURI(path, auth_);
  int err = req_http_->put(uri.c_str(), kApplicationType, buf);
  if (err != 0) {
    error_ = FirebaseError(err, "HTTP request failed");
    return;
  }
  int statusCode = req_http_->responseStatusCode();
  if (statusCode != kStatusOK) {
    error_ = FirebaseError(statusCode, "POST request failed");
    return;
  }
  req_http_->responseBody(); // consume body;
  delete buf;
}

void FirebaseArduino::getRequest(const String& path) {
  initRequest();
  String uri = makeFirebaseURI(path, auth_);
  int err = req_http_->get(uri.c_str());
  if (err != 0) {
    error_ = FirebaseError(err, "HTTP request failed");
    return;
  }
  int statusCode = req_http_->responseStatusCode();
  if (statusCode != kStatusOK) {
    error_ = FirebaseError(err, "GET request failed");
    return;
  }
}

FirebaseObject FirebaseArduino::get(const String& path) {
  getRequest(path);
  if (failed()) {
    return FirebaseObject{""};
  }
  return FirebaseObject(req_http_->responseBody());
}

int FirebaseArduino::getInt(const String& path) {
  getRequest(path);
  if (failed()) {
    return 0;
  }
  return FirebaseObject(req_http_->responseBody()).getInt();
}


float FirebaseArduino::getFloat(const String& path) {
  getRequest(path);
  if (failed()) {
    return 0.0f;
  }
  return FirebaseObject(req_http_->responseBody()).getFloat();
}

String FirebaseArduino::getString(const String& path) {
  getRequest(path);
  if (failed()) {
    return "";
  }
  return FirebaseObject(req_http_->responseBody()).getString();
}

bool FirebaseArduino::getBool(const String& path) {
  getRequest(path);
  if (failed()) {
    return "";
  }
  return FirebaseObject(req_http_->responseBody()).getBool();
}

void FirebaseArduino::remove(const String& path) {
  initRequest();
  String uri = makeFirebaseURI(path, auth_);
  int err = req_http_->del(uri.c_str());
  if (err != 0) {
    error_ = FirebaseError(err, "HTTP request failed");
    return;
  }
  int statusCode = req_http_->responseStatusCode();
  if (statusCode != kStatusOK) {
    error_ = FirebaseError(statusCode, "PUT request failed");
    return;
  }
  req_http_->responseBody(); // consume body;
}

void FirebaseArduino::stream(const String& path) {
  initStream();
  String uri = makeFirebaseURI(path, auth_);
  stream_http_->beginRequest();
  stream_http_->get(uri.c_str());
  stream_http_->sendHeader("Accept", "text/event-stream");
  stream_http_->endRequest();

  int statusCode = stream_http_->responseStatusCode();
  if (statusCode != kStatusOK) {
    error_ = FirebaseError(statusCode, "STREAM request failed");
    return;
  }

  if (statusCode == kStatusTemporaryRedirect) {
    while(stream_http_->headerAvailable()) {
      if (stream_http_->readHeaderName() == "Location") {
        String location = stream_http_->readHeaderValue();
        int hostnameStart = location.indexOf(':')+2;
        int hostnameEnd = location.indexOf('/', hostnameStart);
        String hostname = location.substring(hostnameStart, hostnameEnd);
        String path = location.substring(hostnameEnd);
        client_.stop();
        stream_http_.reset(new HttpClient(client_, hostname, kFirebasePort));
        stream_http_->connectionKeepAlive();
        stream(path);
        return;
      }
    }
  }
}

bool FirebaseArduino::available() {
  if (stream_http_.get() == nullptr) {
    return false;
  }
  return stream_http_->available();
}

FirebaseObject FirebaseArduino::readEvent() {
  if (stream_http_.get() == nullptr) {
    return FirebaseObject("");
  }
  String type = stream_http_->readStringUntil('\n').substring(7);;
  String event = stream_http_->readStringUntil('\n').substring(6);
  stream_http_->readStringUntil('\n'); // consume separator
  FirebaseObject obj = FirebaseObject(event);
  obj.getJsonVariant().asObject()["type"] = type.c_str();
  return obj;
}

bool FirebaseArduino::success() {
  return error_.code() == 0;
}

bool FirebaseArduino::failed() {
  return error_.code() != 0;
}

const String& FirebaseArduino::error() {
  return error_.message().c_str();
}


#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

#include <WiFiClientSecure.h>
WiFiClientSecure client;
FirebaseArduino Firebase(client);

#endif
