#include "modem/commands.h"

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
  std::unique_ptr<FirebaseGet> get(fbase().getPtr(path));

  if (get->error()) {
    out->print("-FAIL ");
    out->println(get->error().message());
    return false;
  }

  String value(get->response());
  // TODO implement json parsing to pull and process value.
  out->print("+");
  out->println(value);
  return true;
}

}  // modem
}  // firebase
