#ifndef MODEM_DB_COMMANDS_H
#define MODEM_DB_COMMANDS_H

#include "FirebaseArduino.h"
#include "modem/command.h"
#include "modem/output-stream.h"
#include "modem/input-stream.h"

namespace firebase {
namespace modem {

class GetCommand : public Command {
 public:
  GetCommand(FirebaseArduino* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class SetCommand : public Command {
 public:
  SetCommand(FirebaseArduino* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class RemoveCommand : public Command {
 public:
  RemoveCommand(FirebaseArduino* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class PushCommand : public Command {
 public:
  PushCommand(FirebaseArduino* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class BeginCommand : public Command {
 public:
  BeginCommand() : Command(nullptr) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);

  // This can only be called once.
  std::unique_ptr<FirebaseArduino> firebase();

 private:
  std::unique_ptr<FirebaseArduino> new_firebase_;
};

class StreamCommand : public Command {
 public:
  StreamCommand(FirebaseArduino* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

}  // modem
}  // firebase

#endif //MODEM_DB_COMMANDS_H
