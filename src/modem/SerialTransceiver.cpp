#include "SerialTransceiver.h"

namespace firebase {
namespace modem {

void SerialTransceiver::begin(Stream* serial) {
  in_.reset(new ArduinoInputStream(serial));
  out_.reset(new ArduinoOutputStream(serial));
}

void SerialTransceiver::RegisterProtocol(std::unique_ptr<SerialProtocol> protocol) {
  for (const String& command : protocol->commands()) {
    command_to_protocol_.insert({command, protocol.get()});
  }

  protocols_.push_back(std::move(protocol));
}

void SerialTransceiver::loop() {
  String command_name = in_->readStringUntil(' ');

  if (command_name.length() == 0 // Generally means a timeout has occured.
      || command_name == "\n") {
    return;
  }

  auto itr = command_to_protocol_.find(command_name);
  if (itr == command_to_protocol_.end()) {
    in_->drain();
    out_->println(String("-FAIL Invalid command '") + command_name + "'." );
    return;
  }
  itr->second->Execute(command_name, in_.get(), out_.get());
}


}  // modem
}  // firebase
