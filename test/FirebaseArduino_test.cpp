//
// Copyright 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "FirebaseObject.h"
#include "gtest/gtest.h"

TEST(FirebaseObjectTest, JsonLiteral) {
  EXPECT_EQ(bool(FirebaseObject("true")), true);
  EXPECT_EQ(bool(FirebaseObject("false")), false);
  EXPECT_EQ(int(FirebaseObject("42")), 42);
  EXPECT_EQ(float(FirebaseObject("43.0")), 43.0);
  EXPECT_EQ(String(FirebaseObject("\"foo\"")), "foo");
}

TEST(FirebaseObjectTest, JsonObject) {
  {
    const JsonObject& obj = FirebaseObject("{\"foo\":\"bar\"}");
    String foo = obj["foo"];
    EXPECT_EQ(foo, "bar");
  }
  {
    String foo = FirebaseObject("{\"foo\":\"bar\"}")["foo"];
    EXPECT_EQ(foo, "bar");
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
