#ifndef MODEM_COMMAND_H
#define MODEM_COMMAND_H

#include "FirebaseArduino.h"
#include "modem/output-stream.h"
#include "modem/input-stream.h"

namespace firebase {
namespace modem {

class Command {
 public:
  Command(FirebaseArduino* fbase) : fbase_(fbase) {}

  // Execute command, reading any additional data needed from stream.
  // Return false if execution failed.
  virtual bool execute(const String& command,
                       InputStream* in, OutputStream* out) = 0;
 protected:
  FirebaseArduino& fbase() {
    return *fbase_;
  }

 private:
  FirebaseArduino* fbase_;
};

}  // modem
}  // firebase

#endif //MODEM_COMMAND_H
