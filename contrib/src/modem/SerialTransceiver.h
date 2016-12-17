#ifndef MODEM_SERIAL_TRANSCIEVER_H
#define MODEM_SERIAL_TRANSCIEVER_H

#include <memory>
#include <map>

#include "modem/SerialProtocol.h"
#include "modem/input-stream.h"
#include "modem/output-stream.h"

namespace firebase {
namespace modem {

class SerialTransceiver {
 public:
  void RegisterProtocol(std::unique_ptr<SerialProtocol> protocol);
  // Also takes ownership as above but more arduino friendly.
  void RegisterProtocol(SerialProtocol* protocol) {
    RegisterProtocol(std::unique_ptr<SerialProtocol>(protocol));
  }

  void begin(Stream* serial);
  void loop();

 private:
  std::unique_ptr<ArduinoInputStream> in_;
  std::unique_ptr<ArduinoOutputStream> out_;
  std::vector<std::unique_ptr<SerialProtocol>> protocols_;
};

}  // modem
}  // firebase

#endif // MODEM_SERIAL_TRANSCIEVER_H
