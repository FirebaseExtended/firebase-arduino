#include "modem/db/DatabaseProtocol.h"

namespace firebase {
namespace modem {
namespace {
const std::vector<std::string> commands {
  "BEGIN_DB",
  "GET",
  "SET",
  "PUSH",
  "REMOVE",
  "BEGIN_STREAM"
}
}

const std::vector<std::string>& DatabaseProtocol::commands() const {
  return commands;
}

void DatabaseProtocol::Execute(const std::string& command, InputStream* in,
                               OutputStream* out) {
  if (command_name == "BEGIN_DB") {
    BeginCommand command;
    if (command.execute(command_name, in_.get(), out_.get())) {
      fbase_ = std::move(command.firebase());
    }
    return;
  } else if (!fbase_) {
    in_->drain();
    out_->println("-FAIL Must call BEGIN_DB before anything else.");
    return;
  }

  std::unique_ptr<Command> command = CreateCommand(command_name, fbase_.get());
  if (!command) {
    in_->drain();
    out_->println(String("-FAIL Invalid command '") + command_name + "'." );
    return;
  }
  
  command->execute(command_name, in_.get(), out_.get());
}

std::unique_ptr<Command> SerialTransceiver::CreateCommand(const String& text,
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
