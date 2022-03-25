#include "Arduino.h"
#include "thing/Config.h"
#include <ArduinoJson.h>

namespace thing {

ConfigJsonSerializer::ConfigJsonSerializer(const Config& config) {
  JsonObject& root = buffer_.createObject();
  root_ = &root;
  root["host"] = config.host.c_str();
  root["auth"] = config.auth.c_str();
  root["path"] = config.path.c_str();
  root["wifi_ssid"] = config.wifi_ssid.c_str();
  root["wifi_key"] = config.wifi_key.c_str();
  root["analog_activation"] = config.analog_activation_threshold;
  root["wifi_connect_attempts"] = config.wifi_connect_attempts;

  JsonObject& pins_root = root.createNestedObject("pins");
  pins_root["digital_in"] = config.pins.digital_in;
  pins_root["digital_out"] = config.pins.digital_out;
  pins_root["analog_in"] = config.pins.analog_in;
  pins_root["analog_out"] = config.pins.analog_out;
  pins_root["config_mode_button"] = config.pins.config_mode_button;
}

ConfigJsonSerializer::ConfigJsonSerializer(char* serialized_config) {
  root_ = &(buffer_.parseObject(serialized_config));
}

int ConfigJsonSerializer::content_length() const {
  return root_->measureLength();
}

void ConfigJsonSerializer::SerializeTo(Stream* output) {
  // TODO: We "should" be able to have the root_ print directly to the stream
  // however it currently closes the connection half way through. Fixing this
  // would save ~250B of memory during serialization.
  String buffer;
  root_->printTo(buffer);
  output->print(buffer);
}

void ConfigJsonSerializer::DeserializeTo(Config* config) {
  config->host = root()["host"].asString();
  config->auth = root()["auth"].asString();
  config->path = root()["path"].asString();
  config->wifi_ssid = root()["wifi_ssid"].asString();
  config->wifi_key = root()["wifi_key"].asString();
  config->analog_activation_threshold = root()["activation_threshold"];
  config->wifi_connect_attempts = root()["wifi_connect_attempts"];

  config->pins.digital_in = root()["pins"]["digital_in"];
  config->pins.digital_out = root()["pins"]["digital_out"];
  config->pins.analog_in = root()["pins"]["analog_in"];
  config->pins.analog_out = root()["pins"]["analog_out"];
  config->pins.config_mode_button = root()["pins"]["config_mode_button"];
}

};

