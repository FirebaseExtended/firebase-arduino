#include "Arduino.h"
#include "thing/Config.h"
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

namespace thing {

void Config::SerializeToJson(Stream* output, std::function<void(int size)> handle_size) const {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["host"] = host.c_str();
  root["auth"] = auth.c_str();
  root["path"] = path.c_str();
  root["wifi_ssid"] = wifi_ssid.c_str();
  root["wifi_key"] = wifi_key.c_str();
  root["analog_activation"] = analog_activation_threshold;
  root["wifi_connect_attempts"] = wifi_connect_attempts;

  JsonObject& pins_root = root.createNestedObject("pins");
  pins_root["digital_in"] = pins.digital_in;
  pins_root["digital_out"] = pins.digital_out;
  pins_root["analog_in"] = pins.analog_in;
  pins_root["analog_out"] = pins.analog_out;
  pins_root["config_mode_button"] = pins.config_mode_button;

  handle_size(root.measureLength());
  root.printTo(*output);
}

void Config::ReadFromJson(const char* string) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(string);
  host = root["host"].asString();
  auth = root["auth"].asString();
  path = root["path"].asString();
  wifi_ssid = root["wifi_ssid"].asString();
  wifi_key = root["wifi_key"].asString();
  analog_activation_threshold = root["activation_threshold"];
  wifi_connect_attempts = root["wifi_connect_attempts"];

  pins.digital_in = root["pins"]["digital_in"];
  pins.digital_out = root["pins"]["digital_out"];
  pins.analog_in = root["pins"]["analog_in"];
  pins.analog_out = root["pins"]["analog_out"];
  pins.config_mode_button = root["pins"]["config_mode_button"];
}

};

