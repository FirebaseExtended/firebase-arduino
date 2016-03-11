#include "gtest/gtest.h"
#include "test/mock-firebase.h"
#include "modem/test/mock-output-stream.h"
#include "modem/test/mock-input-stream.h"
#include "Firebase.h"
#include "modem/commands.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::StartsWith;
using ::testing::_;

class BeginCommandTest : public ::testing::Test {
 protected:
  void FeedCommand(const String& host, const String& auth = "") {
    String command_fragment(String(" ") + host);
    if (!auth.empty()) {
      command_fragment += String(" ") + auth;
    }

    EXPECT_CALL(in_, readLine())
        .WillOnce(Return(command_fragment));
  }

  void ExpectOutput(const String& output) {
    EXPECT_CALL(out_, println(output))
        .WillOnce(Return(3));
  }

  void ExpectOutputStartsWith(const String& output) {
    EXPECT_CALL(out_, println(StartsWith(output)))
        .WillOnce(Return(3));
  }

  MockInputStream in_;
  MockOutputStream out_;
};

TEST_F(BeginCommandTest, hostOnly) {
  const String host("http://test.firebase.com");

  FeedCommand(host);
  ExpectOutput("+OK");

  BeginCommand command;
  ASSERT_TRUE(command.execute("BEGIN", &in_, &out_));

  std::unique_ptr<Firebase> firebase(command.firebase());
  EXPECT_EQ("", firebase->auth());
}

TEST_F(BeginCommandTest, hostAndAuth) {
  const String host("http://test.firebase.com");
  const String auth("afasdfsadfasdssfadsfsd");

  FeedCommand(host, auth);
  ExpectOutput("+OK");

  BeginCommand command;
  ASSERT_TRUE(command.execute("BEGIN", &in_, &out_));

  std::unique_ptr<Firebase> firebase(command.firebase());
  EXPECT_EQ(auth, firebase->auth());
}

TEST_F(BeginCommandTest, neitherHostNorAuth) {
  FeedCommand("");
  ExpectOutputStartsWith("-FAIL");

  BeginCommand command;
  ASSERT_FALSE(command.execute("BEGIN", &in_, &out_));

  std::unique_ptr<Firebase> firebase(command.firebase());
  EXPECT_FALSE(firebase);
}
}  // modem
}  // firebase
