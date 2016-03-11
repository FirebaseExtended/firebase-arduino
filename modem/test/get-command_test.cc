#include "gtest/gtest.h"
#include "modem/test/mock-output-stream.h"
#include "modem/test/mock-input-stream.h"
#include "test/mock-firebase.h"
#include "Firebase.h"
#include "modem/commands.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

TEST(get, parsesPath) {
  const String path("/test/path");
  const String command_fragment(String(" ") + path + "\r\n");

  MockInputStream in;
  EXPECT_CALL(in, readLine())
      .WillOnce(Return(command_fragment));

  std::unique_ptr<MockFirebaseGet> get(new MockFirebaseGet());
  const String value("Test value");
  EXPECT_CALL(*get, json())
      .WillOnce(ReturnRef(value));

  MockOutputStream out;
  EXPECT_CALL(out, print(String("+")))
      .WillOnce(Return(1));

  EXPECT_CALL(out, println(value))
      .WillOnce(Return(1));

  MockFirebase fbase;
  EXPECT_CALL(fbase, getPtr(_))
      .WillOnce(Return(ByMove(std::move(get))));

  GetCommand getCmd(&fbase);
  ASSERT_TRUE(getCmd.execute("GET", &in, &out));
}


}  // modem
}  // firebase
