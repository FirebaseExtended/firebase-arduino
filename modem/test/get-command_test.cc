#include "test/gtest/gtest.h"
#include "modem/test/mock-output-stream.h"

namespace firebase {
namespace modem {

TEST(get, parsesPath) {
  const String path = "/test/path";
  const String command_fragment = String(" ") + path + "\r\n";

  MockStream in;
  EXPECT_CALL(in, readStringUntil("\r\n"))
      .WillOnce(Return(command_fragment));

  MockFirebase fbase;
  MockFirebaseGet get;
  EXPECT_CALL(fbase, get(path))
      .WillOnce(Return(get));

  const String value = "Test value";
  EXPECT_CALL(get, json())
      .WillOnce(Return(value));

  EXPECT_CALL(out, print("+"))
      .WillOnce(Return(1));

  MockOutputStream out;
  EXPECT_CALL(out, println(value))
      .WillOnce(Return(1));

  GetCommand get(&fbase);
  ASSERT_TRUE(get.execute("GET", &in, &out));
}


}  // modem
}  // firebase
