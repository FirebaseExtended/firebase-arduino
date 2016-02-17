#include "Arduino.h"

class Stream {
 public:
  int available() {
    return 0;
  }
  String readStringUntil(const char term __attribute__((unused))) {
    return String();
  }
};
