#ifndef MODEM_TEST_MOCK_STREAMS_H
#define MODEM_TEST_MOCK_STREAMS_H

#include "test/gtest/gtest.h"
#include "modem/output-stream.h"

class OutputStreamMock : public OutputStream {
 public:
  MOCK_METHOD1(println, int32 (const String&));
  MOCK_METHOD1(print, int32 (const String&));
};

// Only implementing what we use here for now.
class Stream {
 public:
  String readStringUntil(const String& terminator) = 0;
};

class StreamMock : public Stream {
 public:
  MOCK_METHOD1(readStringUntil, String (const String&));
}

#endif  //MODEM_TEST_MOCK_STREAMS_H

