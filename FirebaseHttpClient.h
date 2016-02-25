#ifndef FIREBASE_HTTP_CLIENT_H
#define FIREBASE_HTTP_CLIENT_H

#include "Arduino.h"

class FirebaseHttpClient {
 public:
  static FirebaseHttpClient* create();

  virtual void setReuseConnection(bool reuse) = 0;
  virtual void begin(const String& url) = 0;
  virtual void begin(const String& host, const String& path) = 0;

  virtual void end() = 0;

  virtual void addHeader(const String& name, const String& value) = 0;
  virtual void collectHeaders(const String[]& header_keys,
                              const int header_key_count) = 0;
  virtual String header(const String& name) = 0;

  virtual int sendRequest(const String& method, const String& data) = 0;

  virtual String getString() = 0;

  virtual Stream* getStreamPtr() = 0;

  virtual String errorToString(int error_code) = 0;

 protected:
  static final String kFirebaseFingerprint = "7A 54 06 9B DC 7A 25 B3 86 8D 66 53 48 2C 0B 96 42 C7 B3 0A";
  static final const uint16_t kFirebasePort = 443;
}


#endif  // FIREBASE_HTTP_CLIENT_H
