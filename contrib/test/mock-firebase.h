#ifndef TEST_MOCK_FIREBASE_H
#define TEST_MOCK_FIREBASE_H

#include <memory>
#include "gtest/gtest.h"
#include "FirebaseArduino.h"

namespace firebase {
namespace modem {

class MockFirebase : public FirebaseArduino {
 public:
  MOCK_CONST_METHOD0(error, const FirebaseError&());
  MOCK_CONST_METHOD1(getString, String ());
  MOCK_CONST_METHOD2(pushString, void());
  MOCK_CONST_METHOD1(remove, void());
  MOCK_CONST_METHOD2(setString, void());
  MOCK_CONST_METHOD0(available, bool ());
  MOCK_CONST_METHOD0(readEvent, FirebaseObject ());
  MOCK_CONST_METHOD2(begin, void ());
};


class MockFirebaseRequest : public FirebaseRequest {
 public:
  MOCK_CONST_METHOD0(name, const std::string&());
  MOCK_CONST_METHOD0(response, const std::string&());
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
