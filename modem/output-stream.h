#ifndef MODEM_OUTPUT_STREAM_H
#define MODEM_OUTPUT_STREAM_H

#include <Arduino.h>
#include <Serial.h>
#include <Stream.h>

namespace firebase {
namespace modem {

class OutputStream {
 public:
  virtual int println(const String& string) = 0;
  virtual int print(const String& string) = 0;
};

class ArduinoOutputStream : public OutputStream {
 public:
  ArduinoOutputStream(Stream* stream) : stream_(stream) {}

  int println(const String& string) override {
    return stream_->println(string.c_str());
  }

  int print(const String& string) override {
    return stream_->print(string.c_str());
  }
 private:
  Stream* stream_;
};

class SerialOutputStream : public OutputStream {
 public:
  SerialOutputStream() {}

  int println(const String& string) override {
    return Serial.println(string.c_str());
  }

  int print(const String& string) override {
    return Serial.print(string.c_str());
  }
};

}  // modem
}  // firebase

#endif // MODEM_OUTPUT_STREAM
