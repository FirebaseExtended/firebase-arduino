#include "Firebase.h"
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

class PushCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
    push_.reset(new MockFirebasePush());
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
        .WillOnce(Return(output.length()));
  }

  void ExpectErrorOutput(const String& error_message) {
    EXPECT_CALL(out_, print(String("-FAIL ")))
        .WillOnce(Return(5));
    EXPECT_CALL(out_, println(error_message))
        .WillOnce(Return(error_message.length()));
  }

  bool RunExpectingData(const String& data, const FirebaseError& error) {
    EXPECT_CALL(*push_, error())
      .WillRepeatedly(ReturnRef(error));

    EXPECT_CALL(fbase_, pushPtr(_, EncodeForJson(data)))
        .WillOnce(Return(ByMove(std::move(push_))));

    PushCommand pushCmd(&fbase_);
    return pushCmd.execute("PUSH", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
  std::unique_ptr<MockFirebasePush> push_;
};

TEST_F(PushCommandTest, sendsData) {
  const String path("/test/path");
  const String data("This is a test payload.");

  FeedCommand(path, data);
  ExpectOutput("+OK");

  ASSERT_TRUE(RunExpectingData(data, FirebaseError()));
}

TEST_F(PushCommandTest, HandlesError) {
  const String path("/test/path");
  const String data("This is a test payload.");
  FirebaseError error(-200, "Test error.");

  FeedCommand(path, data);
  ExpectErrorOutput(error.message());

  ASSERT_FALSE(RunExpectingData(data, error));
}

}  // modem
}  // firebase

