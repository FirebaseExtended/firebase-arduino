#include <vector>

#include "Firebase.h"
#include "gtest/gtest.h"
#include "modem/SerialTransceiver.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

class MockSerialProtocol : public SerialProtocol {
 public:
  MOCK_CONST_METHOD0(commands, const std::vector<String>&());
  MOCK_METHOD3(Execute, void(const String&, InputStream*, OutputStream*));
};

class MockStream : public Stream {
 public:
  MOCK_METHOD1(readStringUntil, String(char));
};

class SerialTranscieverTest : public ::testing::Test {
};

TEST_F(SerialTranscieverTest, delegatesCommand) {
  const String good_command = "GOOD";
  const std::vector<String> commands{good_command};

  MockStream serial;
  EXPECT_CALL(serial, readStringUntil(' '))
      .WillOnce(Return(good_command));

  MockSerialProtocol* protocol = new MockSerialProtocol();
  EXPECT_CALL(*protocol, commands())
      .WillOnce(ReturnRef(commands));
  EXPECT_CALL(*protocol, Execute(good_command, _, _));

  SerialTransceiver transceiver;
  transceiver.RegisterProtocol(std::unique_ptr<SerialProtocol>(protocol));
  transceiver.begin(static_cast<Stream*>(&serial));
  transceiver.loop();
}

TEST_F(SerialTranscieverTest, doesNotDelegateInvalidCommand) {
  const String good_command = "GOOD";
  const String bad_command = "BAD";
  const std::vector<String> commands{good_command};

  MockStream serial;
  EXPECT_CALL(serial, readStringUntil(' '))
      .WillOnce(Return(bad_command));

  MockSerialProtocol* protocol = new MockSerialProtocol();
  EXPECT_CALL(*protocol, commands())
      .WillOnce(ReturnRef(commands));
  EXPECT_CALL(*protocol, Execute(bad_command, _, _))
      .Times(0);

  SerialTransceiver transceiver;
  transceiver.RegisterProtocol(std::unique_ptr<SerialProtocol>(protocol));
  transceiver.begin(static_cast<Stream*>(&serial));
  transceiver.loop();
}


}  // namespace modem
}  // namespace firebase
