#include "modem/command.h"
#include "modem/util.h"

namespace firebase {
namespace modem {

bool GetCommand::execute(const String& command,
                         Stream* in, OutputStream* out) {
  if (in == null || out == null) {
    return false;
  }

  if (command != "GET") {
    return false;
  }

  String path = in->readStringUntil(kEndLine);
  FirebaseGet get = fbase_->get(path);

  String value = get.json();
  // TODO implement json parsing to pull and process value.
  out->print("+");
  out->println(value);

  return true;
}

}  // modem
}  // firebase
