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

  void begin(const std::string& UNUSED_ARG(url)) override {
  }

  void begin(const std::string& UNUSED_ARG(host), const std::string& UNUSED_ARG(path)) override {
  }

  void end() override {
  }

  void addHeader(const std::string& UNUSED_ARG(name), const std::string& UNUSED_ARG(value)) override {
  }

  void collectHeaders(const char* UNUSED_ARG(header_keys[]), const int UNUSED_ARG(count)) override {
  }

  std::string header(const std::string& UNUSED_ARG(name)) override {
    return "";
  }

  int sendRequest(const std::string& UNUSED_ARG(method), const std::string& UNUSED_ARG(data)) override {
    return 0;
  }

  std::string getString() override {
    return "";
  }

  Stream* getStreamPtr() override {
    return nullptr;
  }

  std::string errorToString(int UNUSED_ARG(error_code)) override {
     return std::string();
  }
};

FirebaseHttpClient* FirebaseHttpClient::create() {
  return new FirebaseHttpClientDummy();
}

