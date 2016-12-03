#ifndef THING_CONFIG_H
#define THING_CONFIG_H

#include "Arduino.h"
#include <string>
#include <functional>

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

  void SerializeToJson(Stream* output, std::function<void(int size)> handle_size) const;
  void ReadFromJson(const char* string);
};

}  // namespace thing

#endif  // THING_CONFIG_H
