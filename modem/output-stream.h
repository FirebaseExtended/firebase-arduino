#ifndef MODEM_OUTPUT_STREAM_H
#define MODEM_OUTPUT_STREAM_H

#include <Arduino.h>
#include <Serial.h>

namespace firebase {
namespace modem {

class OutputStream {
 public:
  virtual int println(const String& string) = 0;
  virtual int print(const String& string) = 0;
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
