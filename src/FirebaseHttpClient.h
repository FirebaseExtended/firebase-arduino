#ifndef FIREBASE_HTTP_CLIENT_H
#define FIREBASE_HTTP_CLIENT_H

#include <string>

#include "Arduino.h"
#include "Stream.h"

struct HttpStatus {
  static const int TEMPORARY_REDIRECT = 307;
};

class FirebaseHttpClient {
 public:
  static FirebaseHttpClient* create();

  virtual void setReuseConnection(bool reuse) = 0;
  virtual void begin(const std::string& url) = 0;
  virtual void begin(const std::string& host, const std::string& path) = 0;

  virtual void end() = 0;

  virtual void addHeader(const std::string& name, const std::string& value) = 0;
  virtual void collectHeaders(const char* header_keys[],
                              const int header_key_count) = 0;
  virtual std::string header(const std::string& name) = 0;

  virtual int sendRequest(const std::string& method, const std::string& data) = 0;

  virtual std::string getString() = 0;

  virtual Stream* getStreamPtr() = 0;

  virtual std::string errorToString(int error_code) = 0;

  virtual bool connected() = 0;

 protected:
  static const uint16_t kFirebasePort = 443;
};

static const char kFirebaseFingerprint[] =
      "0E:50:70:F6:8F:5D:B3:66:B2:37:F5:63:C5:8C:48:FE:75:1D:32:D5"; // 2024-02
//8B:30:7C:08:62:CD:FF:07:0B:02:0D:1F:16:CB:2B:56:01:24:C7:3B for *.asia-southeast1.firebasedatabase.app
//0E:50:70:F6:8F:5D:B3:66:B2:37:F5:63:C5:8C:48:FE:75:1D:32:D5 for *.us-central1.firebasedatabase.app

#endif  // FIREBASE_HTTP_CLIENT_H
