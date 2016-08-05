#ifndef MODEM_DB_COMMANDS_H
#define MODEM_DB_COMMANDS_H

#include "Firebase.h"
#include "modem/command.h"
#include "modem/output-stream.h"
#include "modem/input-stream.h"

namespace firebase {
namespace modem {

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

class PushCommand : public Command {
 public:
  PushCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

class BeginCommand : public Command {
 public:
  BeginCommand() : Command(nullptr) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);

  // This can only be called once.
  std::unique_ptr<Firebase> firebase();

 private:
  std::unique_ptr<Firebase> new_firebase_;
};

class StreamCommand : public Command {
 public:
  StreamCommand(Firebase* fbase) : Command(fbase) {}

  bool execute(const String& command, InputStream* in, OutputStream* out);
};

}  // modem
}  // firebase

#endif //MODEM_DB_COMMANDS_H
