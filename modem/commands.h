#ifndef MODEM_COMMAND_H
#define MODEM_COMMAND_H

#include "Firebase.h"
#include "modem/output-stream.h"
#include "modem/input-stream.h"

namespace firebase {
namespace modem {

class Command {
 public:
  Command(Firebase* fbase) : fbase_(fbase) {}

  // Execute command, reading any additional data needed from stream.
  // Return false if execution failed.
  virtual bool execute(const String& command,
                       InputStream* in, OutputStream* out) = 0;
 protected:
  Firebase& fbase() {
    return *fbase_;
  }

 private:
  Firebase* fbase_;
};

class GetCommand : public Command {
 public:
  GetCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class SetCommand : public Command {
 public:
  SetCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class RemoveCommand : public Command {
 public:
  RemoveCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

}  // modem
}  // firebase

#endif //MODEM_COMMAND_H
