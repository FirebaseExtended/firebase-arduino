#ifndef MODEM_OUTPUT_STREAM_H
#define MODEM_OUTPUT_STREAM_H

namespace firebase {
namespace modem {

class OutputStream {
 public:
  virtual int32 println(const String& string) = 0;
  virtual int32 print(const String& string) = 0;
};

class SerialOutputStream {
 public:
  SerialOutputStream(Serial* serial) : serial_(serial) {}

  int32 println(const String& string) override {
    return serial_->println(string);
  }

  int32 print(const String& string) override {
    return serial_->print(string);
  }

 private:
  Serial* serial_;
};

}  // modem
}  // firebase

#endif // MODEM_OUTPUT_STREAM
