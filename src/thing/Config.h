#ifndef THING_CONFIG_H
#define THING_CONFIG_H

#include "Arduino.h"
#include <string>
#include <functional>
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

namespace thing {

struct Pins {
  int digital_in;
  int digital_out;
  int analog_in;
  int analog_out;
  int config_mode_button;
};

struct Config {
  std::string host;
  std::string auth;
  std::string path;

  std::string wifi_ssid;
  std::string wifi_key;

  // If the change is analog value is less than this amount we don't send an
  // update.
  float analog_activation_threshold;
  int wifi_connect_attempts;

  Pins pins;
};

class ConfigJsonSerializer {
 public:
  ConfigJsonSerializer(const Config& config);
  int content_length() const;
  void SerializeTo(Stream* output);

  // We need a mutable char array here, otherwise a copy will be made.
  static Config Deserialize(char* string);
 private:
  DynamicJsonBuffer json_;
  JsonObject* root_;
};

}  // namespace thing

#endif  // THING_CONFIG_H
