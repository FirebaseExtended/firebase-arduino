#include "modem/db/commands.h"

namespace firebase {
namespace modem {

bool RemoveCommand::execute(const String& command,
                            InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "REMOVE") {
    return false;
  }

  String path = in->readLine();
  fbase().remove(path);

  if (fbase().error().length() != 0) {
    out->print("-FAIL ");
    out->println(fbase().error().c_str());
    return false;
  }

  out->println("+OK");
  return true;
}

}  // modem
}  // firebase
