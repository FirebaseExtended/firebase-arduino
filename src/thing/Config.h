#ifndef THING_CONFIG_H
#define THING_CONFIG_H

namespace thing {

struct Config {
  std::string host;
  std::string auth;
  std::string path;

  std::string wifi_ssid;
  std::string wifi_key;

  // If the change is analog value is less than this amount we don't send an
  // update.
  float analog_activation_threshold;

  int pin_digital_in;
  int pin_digital_out;
  int pin_analog_in;
  int pin_analog_out;
};

}  // namespace thing

#endif  // THING_CONFIG_H
