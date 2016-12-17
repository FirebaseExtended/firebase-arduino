#ifndef THING_PORTAL_H
#define THING_PORTAL_H

#include "ESP8266WebServer.h"
#include "thing/Config.h"

namespace thing {

class Portal {
 public:
  Portal();

  void Start(const Config& config);
  void Loop();
  void NotifyOnUpdate(std::function<void(const Config& config)> cb);

  void SetDebugHandler(std::function<void(const char* message)> handler);

 private:
  Config config_;
  ESP8266WebServer server_;
  std::function<void(const Config& config)> callback_;
  std::function<void(const char* message)> debug_;

};

}  // namespace thing

#endif  // THING_PORTAL_H
