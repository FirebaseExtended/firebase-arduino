#include "gtest/gtest.h"
#include "test/modem/mock-output-stream.h"
#include "test/modem/mock-input-stream.h"
#include "test/mock-firebase.h"
#include "FirebaseArduino.h"
#include "modem/db/commands.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

class RemoveCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void FeedCommand(const String& path) {
    const String command_fragment(String(" ") + path);
    EXPECT_CALL(in_, readLine())
        .WillOnce(Return(command_fragment));
  }

  bool RunCommand() {
    RemoveCommand command(&fbase_);
    return command.execute("REMOVE", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
};

TEST_F(RemoveCommandTest, success) {
  const String path("/test/path");
  FeedCommand(path);

  const String no_error = "";
  EXPECT_CALL(fbase_, error()).WillOnce(ReturnRef(no_error));

  EXPECT_CALL(out_, println(String("+OK")))
      .WillOnce(Return(3));

  ASSERT_TRUE(RunCommand());
}

TEST_F(RemoveCommandTest, handlesError) {
  const String path("/test/path");
  FeedCommand(path);

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  const String error = "Test Error.";
  EXPECT_CALL(fbase_, error()).WillRepeatedly(ReturnRef(error));

  EXPECT_CALL(out_, println(String(error.c_str())))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand());
}

}  // modem
}  // firebase
