#include "Firebase.h"
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

  const String data = "Test Value";
  const String value(String("{\"path\" : \"/test/path\", \"data\" : \"") + data + "\"}");
  EXPECT_CALL(*stream_, available())
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*stream_, read(_))
      .WillOnce(Invoke([&value](std::string& json) {
        json = value.c_str();
        return FirebaseStream::PUT;
      }));

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

  ASSERT_TRUE(RunCommand(FirebaseError()));
}

TEST_F(StreamCommandTest, handlesError) {
  FirebaseError error(-200, "Test Error.");
  const String path("/test/path");
  EXPECT_CALL(in_, readLine())
      .WillOnce(Return(path));

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(String(error.message().c_str())))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand(error));
}

}  // modem
}  // firebase
