#include "gtest/gtest.h"
#include "modem/test/mock-output-stream.h"
#include "modem/test/mock-input-stream.h"
#include "Firebase.h"
#include "modem/commands.h"

namespace firebase {
namespace modem {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::ReturnRef;
using ::testing::_;

class MockFirebase : public Firebase {
 public:
  MOCK_METHOD1(getPtr, std::unique_ptr<FirebaseGet>(const String&));
};

class MockFirebaseGet : public FirebaseGet {
 public:
  MOCK_CONST_METHOD0(json, const String&());
};

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
      .WillOnce(Return(ByMove(std::unique_ptr<FirebaseGet>(get.release()))));

  GetCommand getCmd(&fbase);
  ASSERT_TRUE(getCmd.execute("GET", &in, &out));
}


}  // modem
}  // firebase
