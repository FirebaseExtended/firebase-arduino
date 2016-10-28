
#include "thing/Transcriber.h"

namespace thing {
namespace {
  const char* kSubPathDigitalOut = "/digital_out";
  const char* kSubPathDigitalIn = "/digital_in";
  const char* kSubPathAnalogOut = "/analog_out";
  const char* kSubPathAnalogIn = "/analog_in";
}  // namespace


Transcriber::Transcriber(const Config& config) {
  Init(config);
}

void Transcriber::UpdateConfig(const Config& config) {
  Init(config);
}

void Transcriber::Init(const Config& config) {
  path_ = config.path;
  pin_digital_out_ = config.pin_digital_out;
  pin_digital_in_ = config.pin_digital_in;
  pin_analog_out_ = config.pin_analog_out;
  pin_analog_in_ = config.pin_analog_in;

  fbase_.reset(new Firebase(config.host, config.auth));
  stream_ = fbase_->streamPtr(path_);
}

void Transcriber::SetValue(const std::string& path, const std::string& value) {
  stream_.reset(nullptr);
  fbase_->set(path, value);
  stream_ = fbase_->streamPtr(path_);
}

void Transcriber::Loop() {
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
  int digital_in = digitalRead(pin_digital_in_);
  if (digital_in != digital_in_) {
    SetValue(path_ + kSubPathDigitalIn, String(digital_in).c_str());
    digital_in_ = digital_in;
  }

  float analog_in = analogRead(pin_analog_in_);
  if (analog_in != analog_in_) {
    SetValue(path_ + kSubPathAnalogIn, String(analog_in).c_str());
    analog_in_ = analog_in;
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

}  // thing
