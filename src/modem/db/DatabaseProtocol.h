#ifndef MDOEM_DB_DATABASE_PROTOCOL_H
#define MODEM_DB_DATABASE_PROTOCOL_H

#include "modem/SerialProtocol.h"
#include "modem/db/commands.h"

namespace firebase {
namespace modem {

class DatabaseProtocol : public SerialProtocol {
 public:
  const std::vector<std::string>& commands() const override;
  void Execute(const std::string& command, InputStream* in, OutputStream* out) override;
 private:
  std::unique_ptr<Command> CreateCommand(const String& text, Firebase* fbase);
};


}  // modem
}  // firebase


#endif  // MODEM_DB_DATABASE_PROTOCOL_H
