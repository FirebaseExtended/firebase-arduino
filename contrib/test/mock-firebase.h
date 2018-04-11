#ifndef TEST_MOCK_FIREBASE_H
#define TEST_MOCK_FIREBASE_H

#include <memory>
#include "gtest/gtest.h"
#include "Firebase.h"

namespace firebase {
namespace modem {

class MockFirebase : public Firebase {
 public:
};


class MockFirebaseRequest : public FirebaseRequest {
 public:
  MOCK_CONST_METHOD0(name, const std::string&());
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
