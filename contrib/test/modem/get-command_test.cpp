#include "Firebase.h"
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
    get_.reset(new MockFirebaseGet());
  }

  void FeedCommand(const String& path) {
    const String command_fragment(String(" ") + path);
    EXPECT_CALL(in_, readLine())
        .WillOnce(Return(command_fragment));
  }

  bool RunCommand(const FirebaseError& error) {
    EXPECT_CALL(*get_, error())
      .WillRepeatedly(ReturnRef(error));

    EXPECT_CALL(fbase_, getPtr(_))
        .WillOnce(Return(ByMove(std::move(get_))));

    GetCommand getCmd(&fbase_);
    return getCmd.execute("GET", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
  std::unique_ptr<MockFirebaseGet> get_;
};

TEST_F(GetCommandTest, gets) {
  const String path("/test/path");
  FeedCommand(path);

  const String value("Test value");
  EXPECT_CALL(*get_, response())
      .WillOnce(ReturnRef(value));

  EXPECT_CALL(out_, print(String("+")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(value))
      .WillOnce(Return(1));

  ASSERT_TRUE(RunCommand(FirebaseError()));
}

TEST_F(GetCommandTest, handlesError) {
  FirebaseError error(-200, "Test Error.");
  const String path("/test/path");
  FeedCommand(path);

  EXPECT_CALL(out_, print(String("-FAIL ")))
      .WillOnce(Return(1));

  EXPECT_CALL(out_, println(String(error.message().c_str())))
      .WillOnce(Return(1));
  ASSERT_FALSE(RunCommand(error));

}

}  // modem
}  // firebase
