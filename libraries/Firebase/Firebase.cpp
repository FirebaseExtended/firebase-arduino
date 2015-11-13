#include "Firebase.h"
#include "Client.h"

FirebaseRef::FirebaseRef(FirebaseRoot& root, const String& path) : _root(root), _path(path) {
}

FirebaseRef& FirebaseRef::root()  {
  return _root;
}

String FirebaseRef::value() {
  return _root.makeRequest("GET", _path, "");
}

String FirebaseRef::set(const String& key, const String& value) {
  return _root.makeRequest("PUSH", _path + "/" + key, value);
}

String FirebaseRef::push(const String& value) {
  return _root.makeRequest("POST", _path, value);
}

FirebaseRef FirebaseRef::child(const String& key) {
  return FirebaseRef(_root, _path + "/" + key);
}

FirebaseRoot::FirebaseRoot() : FirebaseRef(*this, "") {
}

void FirebaseRoot::begin(Client& client, const String& host) {
  _client = &client;
  _host = host;
}

void FirebaseRoot::auth(const String& token) {
  _token = token;
}

void FirebaseRoot::setError(const String& err) {
  _err = err;
}

const String& FirebaseRoot::error() {
  return _err;
}

String FirebaseRoot::makeRequest(const String& method, const String& path, const String& data) {
  _err = "";
  
  if (!_client->connect(_host.c_str(), 443)) {
    setError("failed to connect");
    return "";
  }    
  String req;
  req += method + " " + path + ".json";
  if (_token.length() > 0) {
    req += "?auth=" + _token;
  }
  req += " HTTP/1.1\r\n";
  req += "Host: " + _host + "\r\n";
  req += "User-Agent: Arduino\r\n";
  req += "Connection: close\r\n";
  if (data.length()) {
    req += "Content-Length: ";
    req += data.length();
    req += "\r\n\r\n";
    req += data;
  }
  _client->print(req);
  String resp;
  while(_client->available() > 0) {
    resp += _client->readStringUntil('\n');
  }
  return resp;
}

FirebaseRoot Firebase;
