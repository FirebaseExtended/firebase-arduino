#include "FirebaseArduino.h"
#include "gtest/gtest.h"
#include "modem/db/commands.h"
#include "test/modem/mock-input-stream.h"
#include "test/modem/mock-output-stream.h"
#include "test/mock-firebase.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

class GetCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void FeedCommand(const String& path) {
    const String command_fragment(String(" ") + path);
     EXPECT_CALL(in_, readLine())
        .WillOnce(Return(command_fragment));
  }

  bool RunCommand(const FirebaseError& error) {
    GetCommand getCmd(&fbase_);
    return getCmd.execute("GET", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
};

TEST_F(GetCommandTest, gets) {
  const String path("/test/path");
  const String command_fragment(" /test/path");
  FeedCommand(path);

  const String value("Test value");
  EXPECT_CALL(fbase_, getString(command_fragment)).WillOnce(Return("Test value"));

  const String no_error = "";
  EXPECT_CALL(fbase_, error()).WillOnce(ReturnRef(no_error));

  EXPECT_CALL(out_, print(String("+")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(value))
      .WillOnce(Return(1));

  ASSERT_TRUE(RunCommand(FirebaseError()));
}

TEST_F(GetCommandTest, handlesError) {
  FirebaseError error(-200, "Test Error.");
  const String command_fragment(" /test/path");
  const String path("/test/path");
  FeedCommand(path);

  const String error_value = "Test Error.";
  EXPECT_CALL(fbase_, error()).WillRepeatedly(ReturnRef(error_value));

  EXPECT_CALL(fbase_, getString(command_fragment)).WillOnce(Return(""));

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(error_value))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand(error));

}

}  // modem
}  // firebase
