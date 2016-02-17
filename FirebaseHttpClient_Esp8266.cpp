
#include "FirebaseHttpClient.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Detect whether stable version of HTTP library is installed instead of
// master branch and patch in missing status and methods.
#ifndef HTTP_CODE_TEMPORARY_REDIRECT
#define HTTP_CODE_TEMPORARY_REDIRECT 307
#define USE_ESP_ARDUINO_CORE_2_0_0
#endif


class FirebaseHttpClientEsp8266 : public FirebaseHttpClient {
 public:
  FirebaseHttpClientEsp8266() {}

  void SetReuseConnection(bool reuse) override {
    http_.reuse(reuse);
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

  void collectHeaders(const String[]& header_keys, const int count) override {
    http_.collectHeaders(header_keys, count);
  }

  String header(const String& name) override {
    return http_.header(name);
  }

  int sendRequest(const String& method, const String& data) override {
    return http_.sendRequest(method, data);
  }

  String getString() override {
    return http_.getString();
  }

  Stream* getStreamPtr() override {
    return http_.getStreamPtr();
  }

  String errorToStream(int error_code) override {
#ifdef USE_ESP_ARDUINO_CORE_2_0_0
    return  String(status);
#else
    return HTTPClient::errorToString(status);
#endif
  }

 private:
  HTTPClient http_;
};

Firebase* FirebaseHttpClient::create() {
  return new FirebaseHttpClientEsp8266();
}

