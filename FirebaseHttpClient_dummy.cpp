
#include "FirebaseHttpClient.h"

class FirebaseHttpClientDummy : public FirebaseHttpClient {
 public:
  void SetReuseConnection(bool reuse) override {
  }

  void begin(const String& url) override {
  }

  void begin(const String& host, const String& path) override {
  }

  void end() override {
  }

  void addHeader(const String& name, const String& value) override {
  }

  void collectHeaders(const String[]& header_keys, const int count) override {
  }

  String header(const String& name) override {
    return "";
  }

  int sendRequest(const String& method, const String& data) override {
    return 0;
  }

  String getString() override {
    return "";
  }

  Stream* getStreamPtr() override {
    return http_.getStreamPtr();
  }

  String errorToStream(int error_code) override {
     return  String(status);
  }
}

Firebase* FirebaseHttpClient::create() {
  return new FirebaseHttpClientDummy();
}

