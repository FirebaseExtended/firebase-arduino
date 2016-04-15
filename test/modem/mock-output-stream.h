#ifndef MODEM_TEST_MOCK_OUTPUT_STREAM_H
#define MODEM_TEST_MOCK_OUTPUT_STREAM_H

#include "gtest/gtest.h"
#include "modem/output-stream.h"

namespace firebase {
namespace modem {

class MockOutputStream : public OutputStream {
 public:
  MOCK_METHOD1(println, int (const String&));
  MOCK_METHOD1(println, int (const int));
  MOCK_METHOD1(print, int (const String&));
};

}  // modem
}  // firebase

#endif  //MODEM_TEST_MOCK_OUTPUT_STREAM_H

