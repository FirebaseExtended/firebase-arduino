#include "SerialTransceiver.h"

namespace firebase {
namespace modem {

void SerialTransceiver::begin(Stream* serial) {
  std::unique_ptr<Firebase> fbase;

  in_.reset(new ArduinoInputStream(serial));
  out_.reset(new ArduinoOutputStream(serial));
}

void SerialTransceiver::loop() {
  String command_name = in_->readStringUntil(' ');

  if (command_name.length() == 0 // Generally means a timeout has occured.
      || command_name == "\n") {
    return;
  }

  if (command_name == "BEGIN") {
    BeginCommand command;
    if (command.execute(command_name, in_.get(), out_.get())) {
      fbase_ = std::move(command.firebase());
    }
    return;
  } else if (!fbase_) {
    in_->drain();
    out_->println("-FAIL Must call BEGIN before anything else.");
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

}  // modem
}  // firebase
