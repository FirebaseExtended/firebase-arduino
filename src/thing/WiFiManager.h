#ifndef THING_WIFI_MANAGER_H
#define THING_WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>

namespace thing {

class WiFiManager {
 public:
  WiFiManager();

  bool StartAP();
  bool StopAP();
  bool Connect(const std::string& ssid, const std::string& auth);

  void Loop();

  bool connected() const;
  bool ap_up() const { return ap_up_; }

  void SetDebugHandler(std::function<void(const char* message)> handler);
 private:
  bool ap_up_ = false;
  std::unique_ptr<DNSServer> dns_;
  std::function<void(const char* message)> debug_;
};

};

#endif // THING_WIFI_MANAGER_H
