#include "modem/db/DatabaseProtocol.h"

namespace firebase {
namespace modem {
namespace {
const std::vector<String> kCommands {
  "BEGIN_DB",
  "GET",
  "SET",
  "PUSH",
  "REMOVE",
  "BEGIN_STREAM"
};
}

const std::vector<String>& DatabaseProtocol::commands() const {
  return kCommands;
}

void DatabaseProtocol::Execute(const String& command_name, InputStream* in,
                               OutputStream* out) {
  if (command_name == "BEGIN_DB") {
    BeginCommand command;
    if (command.execute(command_name, in, out)) {
      fbase_ = std::move(command.firebase());
    }
    return;
  } else if (!fbase_) {
    in->drain();
    out->println("-FAIL Must call BEGIN_DB before anything else.");
    return;
  }

  std::unique_ptr<Command> command = CreateCommand(command_name, fbase_.get());
  if (!command) {
    in->drain();
    out->println(String("-FAIL unhandled command '") + command_name + "'." );
    return;
  }
  
  command->execute(command_name, in, out);
}

std::unique_ptr<Command> DatabaseProtocol::CreateCommand(const String& text,
                                                         Firebase* fbase) {
  std::unique_ptr<Command> command;
  if (text == "GET") {
    command.reset(new GetCommand(fbase));
  } else if (text == "SET") {
    command.reset(new SetCommand(fbase));
  } else if (text == "PUSH") {
    command.reset(new PushCommand(fbase));
  } else if (text == "REMOVE") {
    command.reset(new RemoveCommand(fbase));
  } else if (text == "BEGIN_STREAM") {
    command.reset(new StreamCommand(fbase));
  }
  return command;
}
} // modem
} // firebase
