#ifndef MODEM_SERIAL_TRANSCIEVER_H
#define MODEM_SERIAL_TRANSCIEVER_H

#include <memory>

#include "Firebase.h"
#include "modem/commands.h"

namespace firebase {
namespace modem {

class SerialTransciever {
 public:
  void Begin(Stream* serial);

 private:
  std::unique_ptr<Command> GetCommand(const String& name, Firebase* fbase);
}

}  // modem
}  // firebase

#endif // MODEM_SERIAL_TRANSCIEVER_H
