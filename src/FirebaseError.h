#ifndef firebase_error_h
#define firebase_error_h

class FirebaseError {
 public:
  // Make it explicit that the empty constructor mean no error.
  static FirebaseError OK() {
    return FirebaseError();
  }

  FirebaseError() {}
  FirebaseError(int code, const std::string& message) : code_(code), message_(message) {
  }

  operator bool() const { return code_ != 0; }
  int code() const { return code_; }
  const std::string& message() const { return message_; }

 private:
  int code_ = 0;
  std::string message_ = "";
};

#endif  //firebase_error_h
