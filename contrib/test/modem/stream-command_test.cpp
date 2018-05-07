#include "FirebaseArduino.h"
#include "gtest/gtest.h"
#include "modem/db/commands.h"
#include "test/modem/mock-input-stream.h"
#include "test/modem/mock-output-stream.h"
#include "test/mock-firebase.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::Invoke;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

class StreamCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  bool RunCommand() {
    StreamCommand cmd(&fbase_);
    return cmd.execute("BEGIN_STREAM", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
};

TEST_F(StreamCommandTest, streams) {
  const String path("/test/path");

  const String no_error = "";
  EXPECT_CALL(fbase_, error()).WillRepeatedly(ReturnRef(no_error));

  const String data = "TestValue";
  const String value(String("{\"path\":\"/test/path\",\"data\":\"") + data + "\",\"type\":\"PUT\"}");

  const FirebaseObject fo = FirebaseObject(value.c_str());
  EXPECT_CALL(fbase_, readEvent()).WillRepeatedly(Return(fo));

  EXPECT_CALL(in_, available())
      .WillRepeatedly(Return(true));

  EXPECT_CALL(in_, readLine())
      .WillOnce(Return(path))
      .WillOnce(Return("END_STREAM"));

  EXPECT_CALL(fbase_, available())
     .WillOnce(Return(true))
     .WillRepeatedly(Return(false));
  EXPECT_CALL(fbase_, stream(path));

  EXPECT_CALL(out_, print(String("+")))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, print(String("PUT")))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, print(String(" ")))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, println(path))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(data.length()))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, println(data))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(String("+OK")))
      .WillOnce(Return(1));

  ASSERT_TRUE(RunCommand());
}

TEST_F(StreamCommandTest, handlesError) {
  const String error("Test Error.");
  const String path("/test/path");
  EXPECT_CALL(in_, readLine())
      .WillOnce(Return(path));

  EXPECT_CALL(fbase_, error()).WillRepeatedly(ReturnRef(error));

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(String(error.c_str())))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand());
}

}  // modem
}  // firebase
