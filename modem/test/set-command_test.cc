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
using ::testing::_;

TEST(Set, sendsData) {
  const String path("/test/path");

  const String data("This is a test payload.");
  const String data_fragment(String(" ") + data + "\r\n");

  MockInputStream in;
  EXPECT_CALL(in, readStringUntil(' '))
      .WillOnce(Return(path));
  EXPECT_CALL(in, readLine())
      .WillOnce(Return(data_fragment));

  std::unique_ptr<MockFirebaseSet> set(new MockFirebaseSet());
  FirebaseError error;
  EXPECT_CALL(*set, error())
      .WillOnce(ReturnRef(error));

  MockOutputStream out;
  EXPECT_CALL(out, println(String("+OK")))
      .WillOnce(Return(3));

  MockFirebase fbase;
  EXPECT_CALL(fbase, setPtr(_, data))
      .WillOnce(Return(ByMove(std::move(set))));

  SetCommand setCmd(&fbase);
  ASSERT_TRUE(setCmd.execute("SET", &in, &out));
}

}  // modem
}  // firebase
