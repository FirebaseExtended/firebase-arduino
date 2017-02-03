#include "SerialTransceiver.h"

#include <algorithm>

namespace firebase {
namespace modem {

void SerialTransceiver::begin(Stream* serial) {
  in_.reset(new ArduinoInputStream(serial));
  out_.reset(new ArduinoOutputStream(serial));
}

void SerialTransceiver::RegisterProtocol(std::unique_ptr<SerialProtocol> protocol) {
  protocols_.push_back(std::move(protocol));
}

void SerialTransceiver::loop() {
  String command_name = in_->readStringUntil(' ');

  if (command_name.length() == 0 // Generally means a timeout has occured.
      || command_name == "\n") {
    return;
  }

  bool command_found = false;
  for (auto& protocol : protocols_) {
    const std::vector<String>& commands = protocol->commands();
    if (std::binary_search(commands.begin(), commands.end(), command_name)) {
      protocol->Execute(command_name, in_.get(), out_.get());
      command_found = true;
      break;
    }
  }

  if (!command_found) {
    in_->drain();
    out_->println(String("-FAIL Invalid command '") + command_name + "'." );
    return;
  }
}


}  // modem
}  // firebase
