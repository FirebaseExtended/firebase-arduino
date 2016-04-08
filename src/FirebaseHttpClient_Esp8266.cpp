
#include "FirebaseHttpClient.h"

// The ordering of these includes matters greatly.
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Detect whether stable version of HTTP library is installed instead of
// master branch and patch in missing status and methods.
#ifndef HTTP_CODE_TEMPORARY_REDIRECT
#define HTTP_CODE_TEMPORARY_REDIRECT 307
#define USE_ESP_ARDUINO_CORE_2_0_0
#endif

class FirebaseHttpClientEsp8266 : public FirebaseHttpClient {
 public:
  FirebaseHttpClientEsp8266() {}

  void setReuseConnection(bool reuse) override {
    http_.setReuse(reuse);
  }

  void begin(const String& url) override {
    http_.begin(url, kFirebaseFingerprint);
  }

  void begin(const String& host, const String& path) override {
    http_.begin(host, kFirebasePort, path, true, kFirebaseFingerprint);
  }

  void end() override {
    http_.end();
  }

  void addHeader(const String& name, const String& value) override {
    http_.addHeader(name, value);
  }

  void collectHeaders(const char* header_keys[], const int count) override {
    http_.collectHeaders(header_keys, count);
  }

  String header(const String& name) override {
    return http_.header(name.c_str());
  }

  int sendRequest(const String& method, const String& data) override {
    return http_.sendRequest(method.c_str(), (uint8_t*)data.c_str(), data.length());
  }

  String getString() override {
    return http_.getString();
  }

  Stream* getStreamPtr() override {
    return http_.getStreamPtr();
  }

  String errorToString(int error_code) override {
#ifdef USE_ESP_ARDUINO_CORE_2_0_0
    return  String(error_code);
#else
    return HTTPClient::errorToString(error_code);
#endif
  }

 private:
  HTTPClient http_;
};

FirebaseHttpClient* FirebaseHttpClient::create() {
  return new FirebaseHttpClientEsp8266();
}

