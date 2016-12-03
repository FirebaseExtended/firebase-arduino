#include "thing/FireThing.h"
#include "Arduino.h"
#include "FS.h"

namespace thing {
namespace {

Config kDefaultConfig = {
  "",  // firebase host
  "",  // firebase auth
  "/fthing",  // path in firebase
  "",  // wifi ssid
  "",  // wifi key
  0.1,  // analog activation threshold
  D1,  // digital in
  BUILTIN_LED,  // digital out
  A0,  // analog in
  D1, // analog out
  D0, // config mode button
};

}  // namespace

FireThing::FireThing() : debug_([](const char*) {}) {}

bool FireThing::Setup() {
  Config config;
  if (!ReadConfigFromStorage(&config)) {
    debug_("Failed to read config from storage.");
    return false;
  }
  SetPinModes(config);

  if (digitalRead(config.pins.config_mode_button) || !ConnectToWiFi(config)) {
    wifi_.StartAP();
  }

  portal_.NotifyOnUpdate([this](const Config& config) {
    if (!WriteConfigToStorage(config)) {
      debug_("Failed to write config to storage.");
    }
    SetPinModes(config);
    transcriber_.UpdateConfig(config);
    ConnectToWiFi(config);
  });
  portal_.Start(config);
}

void FireThing::Loop() {
  wifi_.Loop();
  portal_.Loop();
  transcriber_.Loop();
}

bool FireThing::ConnectToWiFi(const Config& config) {
  debug_("Connecting to wifi:");
  debug_(config.wifi_ssid.c_str());
  debug_(config.wifi_key.c_str());
  if (wifi_.Connect(config.wifi_ssid, config.wifi_key)) {
    debug_("Connected");
    return true;
  }
  debug_("Failed to Connect.");
  return false;
}

void FireThing::SetPinModes(const Config& config) {
  pinMode(config.pins.digital_in, INPUT);
  pinMode(config.pins.digital_out, OUTPUT);
  pinMode(config.pins.analog_in, INPUT);
  pinMode(config.pins.analog_out, OUTPUT);

  pinMode(config.pins.config_mode_button, INPUT);
}

bool FireThing::ReadConfigFromStorage(Config* config) {
  if (!SPIFFS.begin()) {
    debug_("Failed to mount FS.");
    return false;
  }

  if (!SPIFFS.exists("fthing.cfg")) {
    debug_("Config not found, using default.");
    *config = kDefaultConfig;
  } else {
    File cfg = SPIFFS.open("fthing.cfg", "r");
    if (!cfg) {
      debug_("Failed to open config for read");
      SPIFFS.end();
      return false;
    }
    config->ReadFromJson(cfg.readString().c_str());
    debug_("Config read from disk.");
  }

  SPIFFS.end();
  return true;
}

bool FireThing::WriteConfigToStorage(const Config& config) {
  if (!SPIFFS.begin()) {
    debug_("Failed to mount FS.");
    return false;
  }

  File cfg = SPIFFS.open("fthing.cfg", "w");
  if (!cfg) {
    debug_("Failed to open config for write");
    SPIFFS.end();
    return false;
  }
  config.SerializeToJson(&cfg, [](int){});

  SPIFFS.end();
  return true;
}

void FireThing::SetDebugHandler(std::function<void(const char* message)> debug) {
  debug_ = debug;
  wifi_.SetDebugHandler(debug);
  portal_.SetDebugHandler(debug);
  transcriber_.SetDebugHandler(debug);
}

}  // namespace thing
