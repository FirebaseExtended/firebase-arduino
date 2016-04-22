#ifndef TEST_MOCK_FIREBASE_H
#define TEST_MOCK_FIREBASE_H

#include <memory>
#include "gtest/gtest.h"
#include "Firebase.h"

namespace firebase {
namespace modem {

class MockFirebase : public Firebase {
 public:
  MOCK_METHOD1(getPtr, std::unique_ptr<FirebaseGet>(const String&));
  MOCK_METHOD2(setPtr, std::unique_ptr<FirebaseSet>(const String&, const String&));
  MOCK_METHOD2(pushPtr, std::unique_ptr<FirebasePush>(const String&, const String&));
  MOCK_METHOD1(removePtr, std::unique_ptr<FirebaseRemove>(const String&));
  MOCK_METHOD1(streamPtr, std::unique_ptr<FirebaseStream>(const String&));
};

class MockFirebaseGet : public FirebaseGet {
 public:
  MOCK_CONST_METHOD0(response, const String&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseSet : public FirebaseSet {
 public:
  MOCK_CONST_METHOD0(json, const String&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebasePush : public FirebasePush {
 public:
  MOCK_CONST_METHOD0(name, const String&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseRemove : public FirebaseRemove {
 public:
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseStream : public FirebaseStream {
 public:
  MOCK_METHOD0(available, bool());
  MOCK_METHOD1(read, Event(String& event));
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

}  // modem
}  // firebase
#endif  // TEST_MOCK_FIREBASE_H
