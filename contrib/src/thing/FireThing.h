#ifndef THING_FIRETHING_H
#define THING_FIRETHING_H

#include "thing/Config.h"
#include "thing/WiFiManager.h"
#include "thing/Transcriber.h"
#include "thing/Portal.h"

namespace thing {

// Operates the device as a FireThing which manages a connection
// between several pins and a firebase database. The configuration
// will be read from and persisted too flash.
class FireThing {
 public:
  FireThing();
  bool Setup();
  void Loop();

  void SetDebugHandler(std::function<void(const char* message)> debug);

  // Called to delete the currently stored config from the filesystem.
  bool DeleteStoredConfig();

 private:
  bool ReadConfigFromStorage(Config* config);
  bool WriteConfigToStorage(const Config& config);

  bool ConnectToWiFi(const Config& config);
  void SetPinModes(const Config& config);

  WiFiManager wifi_;
  Portal portal_;
  Transcriber transcriber_;
  std::function<void(const char* message)> debug_;
};

}  // namespace thing


#endif  // THING_FIRETHING_H
