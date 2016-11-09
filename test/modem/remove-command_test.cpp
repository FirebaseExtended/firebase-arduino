#include "gtest/gtest.h"
#include "test/modem/mock-output-stream.h"
#include "test/modem/mock-input-stream.h"
#include "test/mock-firebase.h"
#include "Firebase.h"
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
    remove_.reset(new MockFirebaseRemove());
  }

  void FeedCommand(const String& path) {
    const String command_fragment(String(" ") + path);
    EXPECT_CALL(in_, readLine())
        .WillOnce(Return(command_fragment));
  }

  bool RunCommand(const FirebaseError& error) {
    EXPECT_CALL(*remove_, error())
      .WillRepeatedly(ReturnRef(error));

    EXPECT_CALL(fbase_, removePtr(_))
        .WillOnce(Return(ByMove(std::move(remove_))));

    RemoveCommand command(&fbase_);
    return command.execute("REMOVE", &in_, &out_);
  }

  MockInputStream in_;
  MockOutputStream out_;
  MockFirebase fbase_;
  std::unique_ptr<MockFirebaseRemove> remove_;
};

TEST_F(RemoveCommandTest, success) {
  const String path("/test/path");
  FeedCommand(path);

  EXPECT_CALL(out_, println(String("+OK")))
      .WillOnce(Return(3));

  ASSERT_TRUE(RunCommand(FirebaseError()));
}

TEST_F(RemoveCommandTest, handlesError) {
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
