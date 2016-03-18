#ifdef __GNUC__
#  define UNUSED_ARG(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED_ARG(x) UNUSED_ ## x
#endif

#include "FirebaseHttpClient.h"

class FirebaseHttpClientDummy : public FirebaseHttpClient {
 public:
  void setReuseConnection(bool UNUSED_ARG(reuse)) override {
  }

  void begin(const String& UNUSED_ARG(url)) override {
  }

  void begin(const String& UNUSED_ARG(host), const String& UNUSED_ARG(path)) override {
  }

  void end() override {
  }

  void addHeader(const String& UNUSED_ARG(name), const String& UNUSED_ARG(value)) override {
  }

  void collectHeaders(const char* UNUSED_ARG(header_keys[]), const int UNUSED_ARG(count)) override {
  }

  String header(const String& UNUSED_ARG(name)) override {
    return "";
  }

  int sendRequest(const String& UNUSED_ARG(method), const String& UNUSED_ARG(data)) override {
    return 0;
  }

  String getString() override {
    return "";
  }

  Stream* getStreamPtr() override {
    return nullptr;
  }

  String errorToString(int UNUSED_ARG(error_code)) override {
     return String();
  }
};

FirebaseHttpClient* FirebaseHttpClient::create() {
  return new FirebaseHttpClientDummy();
}

