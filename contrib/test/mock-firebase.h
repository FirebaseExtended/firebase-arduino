#ifndef TEST_MOCK_FIREBASE_H
#define TEST_MOCK_FIREBASE_H

#include <memory>
#include "gtest/gtest.h"
#include "Firebase.h"

namespace firebase {
namespace modem {

class MockFirebase : public Firebase {
 public:
  MOCK_METHOD1(getPtr, std::unique_ptr<FirebaseGet>(const std::string&));
  MOCK_METHOD2(setPtr, std::unique_ptr<FirebaseSet>(const std::string&, const std::string&));
  MOCK_METHOD2(pushPtr, std::unique_ptr<FirebasePush>(const std::string&, const std::string&));
  MOCK_METHOD1(removePtr, std::unique_ptr<FirebaseRemove>(const std::string&));
  MOCK_METHOD1(streamPtr, std::unique_ptr<FirebaseStream>(const std::string&));
};

class MockFirebaseGet : public FirebaseGet {
 public:
  MOCK_CONST_METHOD0(response, const std::string&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseSet : public FirebaseSet {
 public:
  MOCK_CONST_METHOD0(json, const std::string&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebasePush : public FirebasePush {
 public:
  MOCK_CONST_METHOD0(name, const std::string&());
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseRemove : public FirebaseRemove {
 public:
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

class MockFirebaseStream : public FirebaseStream {
 public:
  MOCK_METHOD0(available, bool());
  MOCK_METHOD1(read, Event(std::string& event));
  MOCK_CONST_METHOD0(error, const FirebaseError&());
};

}  // modem
}  // firebase
#endif  // TEST_MOCK_FIREBASE_H
