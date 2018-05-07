#include "FirebaseArduino.h"
#include "gtest/gtest.h"
#include "modem/db/commands.h"
#include "modem/json_util.h"
#include "test/modem/mock-input-stream.h"
#include "test/modem/mock-output-stream.h"
#include "test/mock-firebase.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

class SetCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void FeedCommand(const String& path, const String& data) {
    const String data_fragment(data);
    EXPECT_CALL(in_, readStringUntil(' '))
        .WillOnce(Return(path));
    EXPECT_CALL(in_, readLine())
        .WillOnce(Return(data_fragment));
  }

  void ExpectOutput(const String& output) {
    EXPECT_CALL(out_, println(output))
        .WillOnce(Return(3));
  }

  void ExpectErrorOutput(const String& error_message) {
    EXPECT_CALL(out_, print(String("-FAIL ")))
        .WillOnce(Return(5));
    EXPECT_CALL(out_, println(error_message))
        .WillOnce(Return(error_message.length()));
  }

  bool RunCommand() {
    SetCommand setCmd(&fbase_);
    return setCmd.execute("SET", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
};

TEST_F(SetCommandTest, sendsData) {
  const String path("/test/path");
  const String data("This is a test payload.");
  const String no_error = "";
  EXPECT_CALL(fbase_, error()).WillOnce(ReturnRef(no_error));

  FeedCommand(path, data);
  ExpectOutput("+OK");

  ASSERT_TRUE(RunCommand());
}

TEST_F(SetCommandTest, HandlesError) {
  const String path("/test/path");
  const String data("This is a test payload.");
  const String error = "TestError";
  EXPECT_CALL(fbase_, error()).WillRepeatedly(ReturnRef(error));

  FeedCommand(path, data);
  ExpectErrorOutput(error);

  ASSERT_FALSE(RunCommand());
}
}  // modem
}  // firebase
