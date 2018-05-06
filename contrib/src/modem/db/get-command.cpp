#include "modem/db/commands.h"

namespace firebase {
namespace modem {

bool GetCommand::execute(const String& command,
                         InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "GET") {
    return false;
  }

  String path = in->readLine();
  String value = fbase().getString(path);
  if (fbase().error().length() != 0) {
    out->print("-FAIL ");
    out->println(fbase().error().c_str());
    return false;
  }

  // TODO implement json parsing to pull and process value.
  out->print("+");
  out->println(value);
  return true;
}

}  // modem
}  // firebase
