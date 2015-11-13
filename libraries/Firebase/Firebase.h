#ifndef firebase_h
#define firebase_h

#include "WString.h"

class Client;
class FirebaseRoot;

class FirebaseRef {
 public:
  FirebaseRef(FirebaseRoot& root, const String& path);
  FirebaseRef& root();
  String value();
  String set(const String& key, const String& value);
  String push(const String& value);
  FirebaseRef child(const String& key);
 private:
  FirebaseRoot& _root;
  String _path;  
};

class FirebaseRoot : public FirebaseRef {
  friend FirebaseRef;
 public:
  FirebaseRoot();
  void begin(Client& client, const String& host);
  void auth(const String& token);
  void setError(const String& err);
  const String& error();
 private:
  String makeRequest(const String& method, const String& path, const String& data);
  Client* _client;
  String _host;
  String _token;
  String _err;  
};

extern FirebaseRoot Firebase;

#endif // firebase_h
