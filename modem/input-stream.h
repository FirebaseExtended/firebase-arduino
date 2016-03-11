#ifndef MODEM_INPUT_STREAM_H
#define MODEM_INPUT_STREAM_H

namespace firebase {
namespace modem {

class InputStream {
 public:
  virtual String readLine() = 0;
  virtual String readStringUntil(const char terminator) = 0;
};

class SerialInputStream : public InputStream {
 public:
  String readLine() override {
    String out = Serial.readStringUntil('\r');
    if (Serial.peek() == '\n') {
      // This should be a '\n' so drop it.
      Serial.read();
    } else {
      // Add \r back, this is not our endline.
      out += '\r';

      // Recurse until \r\n
      out += readLine();
    }
    return out;
  }
  String readStringUntil(const char terminator) {
    return Serial.readStringUntil(terminator);
  }
};

}  // modem
}  // firebase

#endif  // MODEM_INPUT_STREAM_H

