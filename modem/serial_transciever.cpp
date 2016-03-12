#include "modem/serial_transciever.h"

namespace firebase {
namespace modem {

void SerialTransciever::Begin(Stream* serial) {
  std::unique_ptr<Firebase> fbase;

  // Timeout in 5min of waiting for serial.
  serial->setTimeout(300000);

  ArduinoInputStream in(serial);
  ArduinoOutputStream out(serial);
  while(true) {
    String command_name = in->readStringUtil(' ');
    if (command_name.empty()) {
      // Generally means a timeout has occured.
      continue;
    }

    if (command_name == "BEGIN") {
      BeginCommand command;
      if (command.execute(command, in, out)) {
        fbase.reset(command.firebase());
      }
    } else if (!fbase) {
      in.drain();
      out.println("-FAIL Must call BEGIN before anything else.");
      continue;
    }

    std::unique_ptr<Command> command = GetCommand(command_name, fbase.get());
    if (!command) {
      in.drain();
      out.println(String("-FAIL Invalid command '") + command_name + "'." );
      continue;
    }

    command->execute(command_name, &in, &out);
  }
}

std::unique_ptr<Command> GetCommand(const String& text, Firebase* fbase) {
  std::unique_ptr<Command> command;
  if (text == "GET") {
    command.reset(new GetCommand(fbase));
  } else if (text == "SET") {
    command.reset(new SetCommand(fbase));
  } else if (text == "PUSH") {
    command.reset(new PushCommand(fbase));
  } else if (text == "REMOVE") {
    command.reset(new RemoveCommand(fbase));
  }
  return command;
}


}  // modem
}  // firebase
