#ifndef MODEM_SERIAL_TRANSCIEVER_H
#define MODEM_SERIAL_TRANSCIEVER_H

#include <memory>

#include "Firebase.h"
#include "modem/commands.h"

namespace firebase {
namespace modem {

class SerialTransceiver {
 public:
  void begin(Stream* serial);
  void loop();

 private:
  std::unique_ptr<Command> CreateCommand(const String& name, Firebase* fbase);

  std::unique_ptr<Firebase> fbase_;
  std::unique_ptr<ArduinoInputStream> in_;
  std::unique_ptr<ArduinoOutputStream> out_;
};

}  // modem
}  // firebase

#endif // MODEM_SERIAL_TRANSCIEVER_H
