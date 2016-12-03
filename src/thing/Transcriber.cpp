#include "thing/Transcriber.h"
#include <ESP8266WiFi.h>

namespace thing {
namespace {
  const char* kSubPathDigitalOut = "/digital_out";
  const char* kSubPathDigitalIn = "/digital_in";
  const char* kSubPathAnalogOut = "/analog_out";
  const char* kSubPathAnalogIn = "/analog_in";
}  // namespace


Transcriber::Transcriber() : debug_([](const char*) {}) {}

void Transcriber::Setup(const Config& config) {
  Init(config);
}

void Transcriber::UpdateConfig(const Config& config) {
  Init(config);
}

void Transcriber::Init(const Config& config) {
  path_ = config.path;
  analog_activation_threshold_ = config.analog_activation_threshold;
  pin_digital_out_ = config.pins.digital_out;
  pin_digital_in_ = config.pins.digital_in;
  pin_analog_out_ = config.pins.analog_out;
  pin_analog_in_ = config.pins.analog_in;

  fbase_.reset(new Firebase(config.host, config.auth));
}

void Transcriber::SetValue(const std::string& path, const std::string& value) {
  stream_.reset(nullptr);
  fbase_->set(path, value);
  stream_ = fbase_->streamPtr(path_);
}

void Transcriber::Loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connected_ = false;
    return;
  }

  // If first time connecting start stream.
  if (!connected_) {
    debug_("Transcriber connected.");
    stream_ = fbase_->streamPtr(path_);
    connected_ = true;
  }

  // Read incoming data
  if (stream_->available()) {
    std::string event_str;
    if (stream_->read(event_str) == FirebaseStream::PUT) {
      FirebaseObject update(event_str.c_str());
      if (update.success()) {
        if (update.getString("path") == "/") {
          ProcessInitialUpdate(update);
        } else {
          ProcessIncrementalUpdate(update);
        }
      }
    }
  }

  // Send values to cloud
  int new_digital_in = digitalRead(pin_digital_in_);
  if (new_digital_in != digital_in_) {
    SetValue(path_ + kSubPathDigitalIn, String(new_digital_in).c_str());
    digital_in_ = new_digital_in;
  }

  float new_analog_in = analogRead(pin_analog_in_);
  if (abs(new_analog_in - analog_in_) > analog_activation_threshold_) {
    SetValue(path_ + kSubPathAnalogIn, String(new_analog_in).c_str());
    analog_in_ = new_analog_in;
  }
}

void Transcriber::ProcessInitialUpdate(const FirebaseObject& update) {
  int digital_out = update.getInt((std::string("data") + kSubPathDigitalOut).c_str());
  if (!update.failed()) {
    digitalWrite(pin_digital_out_, digital_out);
  }

  float analog_out = update.getFloat((std::string("data") + kSubPathAnalogOut).c_str());
  if (!update.failed()) {
    analogWrite(pin_analog_out_, analog_out);
  }
}

void Transcriber::ProcessIncrementalUpdate(const FirebaseObject& update) {
  if (update.getString("path").equals(kSubPathDigitalOut)) {
    int digital_out = update.getInt("data");
    if (!update.failed()) {
      digitalWrite(pin_digital_out_, update.getInt("data"));
    }
  }

  if (update.getString("path").equals(kSubPathAnalogOut)) {
    float analog_out = update.getFloat("data");
    if (!update.failed()) {
      analogWrite(pin_analog_out_, analog_out);
    }
  }
}

void Transcriber::SetDebugHandler(std::function<void(const char* message)> debug) {
  debug_ = debug;
}

}  // thing
