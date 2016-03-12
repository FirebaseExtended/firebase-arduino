#ifndef MODEM_TEST_MOCK_INPUT_STREAM_H
#define MODEM_TEST_MOCK_INPUT_STREAM_H

#include "gtest/gtest.h"
#include "modem/input-stream.h"

namespace firebase {
namespace modem {

class MockInputStream : public InputStream {
 public:
  MOCK_METHOD0(readLine, String ());
  MOCK_METHOD1(readStringUntil, String (const char));
  MOCK_METHOD0(drain, void ());
  MOCK_METHOD0(available, bool ());
};

}  // modem
}  // firebase

#endif  //MODEM_TEST_MOCK_INPUT_STREAM_H
