#ifndef TEST_MOCK_FIREBASE_H
#define TEST_MOCK_FIREBASE_H

#include <memory>
#include "gtest/gtest.h"
#include "FirebaseArduino.h"

namespace firebase {
namespace modem {

class MockFirebase : public FirebaseArduino {
 public:
  MOCK_METHOD0(error, const String &());
  MOCK_METHOD1(getString, String (const String& path));
  MOCK_METHOD2(pushString, String (const String& path, const String& data));
  MOCK_METHOD1(remove, void(const String& path));
  MOCK_METHOD2(setString, void(const String& path, const String& data));
  MOCK_METHOD0(available, bool ());
  MOCK_METHOD0(readEvent, FirebaseObject ());
  MOCK_METHOD2(begin, void (const String& host, const String& auth));
  MOCK_METHOD1(stream, void (const String& path));
};

}  // modem
}  // firebase
#endif  // TEST_MOCK_FIREBASE_H
