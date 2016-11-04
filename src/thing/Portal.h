#ifndef THING_PORTAL_H
#define THING_PORTAL_H

#include "ESP8266WebServer.h"
#include "thing/Config.h"

namespace thing {

class Portal {
 public:
  Portal(const Config& config);

  void Start();
  void Loop();
  void NotifyOnUpdate(std::function<void(const Config& config)> cb);

 private:
  Config config_;
  ESP8266WebServer server_;
  std::function<void(const Config& config)> callback_;

};

}  // namespace thing

#endif  // THING_PORTAL_H
