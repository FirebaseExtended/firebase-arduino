#ifndef TEST_DUMMIES_STREAM_H
#define TEST_DUMMIES_STREAM_H

#include "Arduino.h"

class Stream {
 public:
  virtual int available() {
    return 0;
  }

  virtual String readStringUntil(const char term __attribute__((unused))) {
    return String();
  }

  virtual int println(const String&) {
    return 0;
  }

  virtual int println(const char*) {
    return 0;
  }

  virtual int println(int) {
    return 0;
  }

  virtual int print(const char*) {
    return 0;
  }

  virtual char peek() {
    return '\0';
  }

  virtual char read() {
    return '\0';
  }
};

#endif  // TEST_DUMMIES_STREAM_H
