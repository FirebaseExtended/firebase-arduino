#ifndef MODEM_INPUT_STREAM_H
#define MODEM_INPUT_STREAM_H

#include <Stream.h>

namespace firebase {
namespace modem {

class InputStream {
 public:
  virtual String readLine() = 0;
  // This call consumes the terminator.
  virtual String readStringUntil(const char terminator) = 0;
  virtual void drain() = 0;
  virtual bool available() = 0;
};

class ArduinoInputStream : public InputStream {
 public:
  ArduinoInputStream(Stream* stream) : stream_(stream) {}
  String readLine() {
    String out = stream_->readStringUntil('\n');
    if (out[out.length()-1] == '\r') {
      out.remove(out.length()-1);
    }
    return out;
  }

  String readStringUntil(const char terminator) {
    return stream_->readStringUntil(terminator);
  }

  void drain() {
    while(stream_->available()) {
      stream_->read();
    }
  }

  bool available() {
    return stream_->available();
  }

 private:
  Stream* stream_;
};

}  // modem
}  // firebase

#endif  // MODEM_INPUT_STREAM_H

