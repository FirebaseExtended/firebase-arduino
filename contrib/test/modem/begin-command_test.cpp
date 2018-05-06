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
using ::testing::StartsWith;
using ::testing::Matcher;
using ::testing::_;

class BeginCommandTest : public ::testing::Test {
 protected:
  void FeedCommand(const String& host, const String& auth = "") {
    String command_fragment(host);
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
    // We have to be explicit here due to the polymorphic nature of println().
    const Matcher<const String&> matcher = StartsWith(output);
    EXPECT_CALL(out_, println(matcher))
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
  ASSERT_TRUE(command.execute("BEGIN_DB", &in_, &out_));

  std::unique_ptr<FirebaseArduino> firebase(command.firebase());
}

TEST_F(BeginCommandTest, hostAndAuth) {
  const String host("http://test.firebase.com");
  const String auth("afasdfsadfasdssfadsfsd");

  FeedCommand(host, auth);
  ExpectOutput("+OK");

  BeginCommand command;
  ASSERT_TRUE(command.execute("BEGIN_DB", &in_, &out_));

  std::unique_ptr<FirebaseArduino> firebase(command.firebase());
}

TEST_F(BeginCommandTest, neitherHostNorAuth) {
  FeedCommand("");
  ExpectOutputStartsWith("-FAIL");

  BeginCommand command;
  ASSERT_FALSE(command.execute("BEGIN_DB", &in_, &out_));

  std::unique_ptr<FirebaseArduino> firebase(command.firebase());
  EXPECT_FALSE(firebase);
}
}  // modem
}  // firebase
