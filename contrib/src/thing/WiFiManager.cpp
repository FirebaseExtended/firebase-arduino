#include "thing/WiFiManager.h"

#include <ESP8266WiFi.h>
#include <DNSServer.h>


namespace thing {
namespace {

const short kDnsPort = 53;

}  // namespace

WiFiManager::WiFiManager() : debug_([](const char*){}) {}

bool WiFiManager::StartAP() {
  // Use arduino string, casting an int to std::string with arduino C++ support
  // isn't easy.
  String ssid("FireThing-");
  ssid += ESP.getChipId();

  WiFi.mode(WIFI_AP_STA);
  if (!WiFi.softAP(ssid.c_str())) {
    return false;
  }

  debug_((String("WiFi AP : ") + ssid).c_str());
  debug_((String("Wifi AP IP : ") + WiFi.softAPIP().toString()).c_str());

  dns_.reset(new DNSServer());
  dns_->start(kDnsPort, "*", WiFi.softAPIP());
  dns_->setTTL(30);

  ap_up_ = true;
  return true;
}

bool WiFiManager::StopAP() {
  if (!WiFi.softAPdisconnect()) {
    return false;
  }
  dns_->stop();
  dns_.reset(nullptr);

  ap_up_ = false;
  return true;
}

void WiFiManager::Loop() {
  if (dns_) {
    dns_->processNextRequest();
  }
}

bool WiFiManager::Connect(const std::string& ssid, const std::string& auth) {
  auto status = WiFi.begin(ssid.c_str(), auth.c_str()); 
  
  return status != WL_CONNECT_FAILED;
}

bool WiFiManager::connected() const {
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::SetDebugHandler(std::function<void(const char* message)> handler) {
  debug_ = std::move(handler);
}

};
