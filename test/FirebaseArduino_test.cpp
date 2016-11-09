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

TEST(FirebaseObjectTest, GetBool) {
  FirebaseObject obj("true");
  EXPECT_EQ(true, obj.getBool());
  EXPECT_TRUE(obj.success());
  EXPECT_FALSE(obj.failed());
  EXPECT_EQ(obj.error(), "");
}

TEST(FirebaseObjectTest, GetInt) {
  {
    FirebaseObject obj("42");
    EXPECT_EQ(42, obj.getInt());
    EXPECT_TRUE(obj.success());
    EXPECT_FALSE(obj.failed());
    EXPECT_EQ(obj.error(), "");
  }
  {
    FirebaseObject obj("42.0");
    EXPECT_EQ(42, obj.getInt());
    EXPECT_TRUE(obj.success());
    EXPECT_FALSE(obj.failed());
    EXPECT_EQ(obj.error(), "");
  }
}

TEST(FirebaseObjectTest, GetFloat) {
  {
    FirebaseObject obj("43.0");
    EXPECT_EQ(43, obj.getFloat());
    EXPECT_TRUE(obj.success());
    EXPECT_FALSE(obj.failed());
    EXPECT_EQ(obj.error(), "");
  }
  {
    FirebaseObject obj("43");
    EXPECT_EQ(43, obj.getFloat());
    EXPECT_TRUE(obj.success());
    EXPECT_FALSE(obj.failed());
    EXPECT_EQ(obj.error(), "");
  }
}

TEST(FirebaseObjectTest, GetString) {
  FirebaseObject obj("\"foo\"");
  EXPECT_EQ("foo", obj.getString());
  EXPECT_TRUE(obj.success());
  EXPECT_FALSE(obj.failed());
  EXPECT_EQ(obj.error(), "");
}

TEST(FirebaseObjectTest, GetObject) {
  {
    FirebaseObject obj("{\"foo\":\"bar\"}");
    EXPECT_EQ(obj.getString("/foo"), "bar");
    EXPECT_EQ(obj.getString("foo"), "bar");
  }
  {
    FirebaseObject obj("{\"foo\": {\"bar\": \"hop\"}}");
    EXPECT_EQ(obj.getString("/foo/bar"), "hop");
  }
}

TEST(FirebaseObjectTest, GetIntFailure) {
  FirebaseObject obj("{\"foo\":\"bar\"}");
  EXPECT_EQ(obj.getInt(), 0);
  EXPECT_FALSE(obj.success());
  EXPECT_TRUE(obj.failed());
  EXPECT_EQ(obj.error(), "failed to convert to number");
}

TEST(FirebaseObjectTest, GetFloatFailure) {
  FirebaseObject obj("{\"foo\":\"bar\"}");
  EXPECT_EQ(obj.getFloat(), 0);
  EXPECT_FALSE(obj.success());
  EXPECT_TRUE(obj.failed());
  EXPECT_EQ(obj.error(), "failed to convert to number");
}

TEST(FirebaseObjectTest, GetBoolFailure) {
  FirebaseObject obj("{\"foo\":\"bar\"}");
  EXPECT_EQ(obj.getBool(), 0);
  EXPECT_FALSE(obj.success());
  EXPECT_TRUE(obj.failed());
  EXPECT_EQ(obj.error(), "failed to convert to bool");
}

TEST(FirebaseObjectTest, GetStringFailure) {
  FirebaseObject obj("{\"foo\":\"bar\"}");
  EXPECT_EQ(obj.getString(), "");
  EXPECT_FALSE(obj.success());
  EXPECT_TRUE(obj.failed());
  EXPECT_EQ(obj.error(), "failed to convert to string");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
