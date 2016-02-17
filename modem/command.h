#ifndef MODEM_COMMAND_H
#define MODEM_COMMAND_H

#include "Firebase.h"
#include "modem/output-stream.h"

namespace firebase {
namespace modem {

class Command {
 public:
  Command(Firebase* fbase) : fbase_(fbase) {}

  // Execute command, reading any additional data needed from stream.
  // Return false if execution failed.
  virtual bool execute(const String& command,
                       Stream* in, OutputStream* out) = 0;

 private:
  Firebase* fbase_;
}

class GetCommand : public Command {
 public:
  GetCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, Stream* stream);
}

}  // modem
}  // firebase

#endif //MODEM_COMMAND_H
