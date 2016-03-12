#ifndef MODEM_INPUT_STREAM_H
#define MODEM_INPUT_STREAM_H

#include <Stream.h>

namespace firebase {
namespace modem {

class InputStream {
 public:
  virtual String readLine() = 0;
  virtual String readStringUntil(const char terminator) = 0;
  virtual void drain() = 0;
};

class ArduinoInputStream : public InputStream {
 public:
  ArduinoInputStream(Stream* stream) : stream_(stream) {}
  String readLine() {
    String out = stream_->readStringUntil('\r');
    if (stream_->peek() == '\n') {
      // This should be a '\n' so drop it.
      stream_->read();
    } else {
      // Add \r back, this is not our endline.
      out += '\r';

      // Recurse until \r\n
      out += readLine();
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

 private:
  Stream* stream_;
};

}  // modem
}  // firebase

#endif  // MODEM_INPUT_STREAM_H

