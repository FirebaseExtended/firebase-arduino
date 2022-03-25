#ifndef MDOEM_DB_DATABASE_PROTOCOL_H
#define MODEM_DB_DATABASE_PROTOCOL_H

#include "modem/SerialProtocol.h"
#include "modem/db/commands.h"
#include "Firebase.h"

namespace firebase {
namespace modem {

class DatabaseProtocol : public SerialProtocol {
 public:
  const std::vector<String>& commands() const override;
  void Execute(const String& command, InputStream* in, OutputStream* out) override;
 private:
  std::unique_ptr<Command> CreateCommand(const String& text, FirebaseArduino* fbase);

  std::unique_ptr<FirebaseArduino> fbase_;
};


}  // modem
}  // firebase


#endif  // MODEM_DB_DATABASE_PROTOCOL_H
