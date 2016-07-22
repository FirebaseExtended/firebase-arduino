#ifndef MODEM_SERIAL_PROTOCOL_H
#define MODEM_SERIAL_PROTOCOL_H

#include <WString.h>
#include <vector>

namespace firebase {
namespace modem {

class InputStream;
class OutputStream;

/*
 * Define generic baseclass for all serial protocols that wish to share
 * the common commandspace.
 */
class SerialProtocol {
 public:
  virtual ~SerialProtocol() = default;

  /*
   * Returns all commands this protocol supports, commands are single words.
   * Returned vector MUST be sorted.
   */
  virtual const std::vector<String>& commands() const = 0;

  /*
   * Execute command, takes over the serial line until execution is done.
   */
  virtual void Execute(const String& command, InputStream* in, OutputStream* out) = 0;

};

}  // modem
}  // firebase

#endif  // MODEM_SERIAL_PROTOCOL_H
