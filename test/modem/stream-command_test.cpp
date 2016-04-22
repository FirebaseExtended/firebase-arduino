#include "Firebase.h"
#include "gtest/gtest.h"
#include "modem/commands.h"
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
    stream_.reset(new MockFirebaseStream());
  }

  bool RunCommand(const FirebaseError& error) {
    EXPECT_CALL(*stream_, error())
      .WillRepeatedly(ReturnRef(error));

    EXPECT_CALL(fbase_, streamPtr(_))
        .WillOnce(Return(ByMove(std::move(stream_))));

    StreamCommand cmd(&fbase_);
    return cmd.execute("BEGIN_STREAM", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
  std::unique_ptr<MockFirebaseStream> stream_;
};

TEST_F(StreamCommandTest, streams) {
  const String path("/test/path");
  EXPECT_CALL(in_, available())
      .WillRepeatedly(Return(true));

  EXPECT_CALL(in_, readLine())
      .WillOnce(Return(path))
      .WillOnce(Return("END_STREAM"));

  const String value("Test value");
  EXPECT_CALL(*stream_, available())
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*stream_, read(_))
      .WillOnce(Invoke([&value](String& json) {
        json = value;
        return FirebaseStream::PUT;
      }));

  EXPECT_CALL(out_, print(String("+")))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, print(String("PUT ")))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, println(String("/dummy/path")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(value.length()))
      .WillOnce(Return(1));
  EXPECT_CALL(out_, println(value))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(String("+OK")))
      .WillOnce(Return(1));

  ASSERT_TRUE(RunCommand(FirebaseError()));
}

TEST_F(StreamCommandTest, handlesError) {
  FirebaseError error(-200, "Test Error.");
  const String path("/test/path");
  EXPECT_CALL(in_, readLine())
      .WillOnce(Return(path));

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(error.message()))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand(error));
}

}  // modem
}  // firebase
