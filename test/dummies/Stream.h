#ifndef TEST_DUMMIES_STREAM_H
#define TEST_DUMMIES_STREAM_H

#include "Arduino.h"

class Stream {
 public:
  int available() {
    return 0;
  }
  String readStringUntil(const char term __attribute__((unused))) {
    return String();
  }
  int println(const String&) {
    return 0;
  }
  int println(const char*) {
    return 0;
  }
  int println(int) {
    return 0;
  }
  int print(const char*) {
    return 0;
  }
  char peek() {
    return '\0';
  }
  char read() {
    return '\0';
  }
};

#endif  // TEST_DUMMIES_STREAM_H
