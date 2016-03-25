#include "modem/commands.h"

namespace firebase {
namespace modem {
namespace {
// TODO(edcoyne): We should use a json library to escape.
String EncodeForJson(String input) {
  input.replace("\\", "\\\\");
  input.replace("\"", "\\\"");
  return "\"" + input + "\"";
}
}  // namespace

bool PushCommand::execute(const String& command,
                         InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "PUSH") {
    return false;
  }

  String path(in->readStringUntil(' '));
  String data(in->readLine());

  std::unique_ptr<FirebasePush> push(
      fbase().pushPtr(path, EncodeForJson(data)));

  if (push->error()) {
    out->print("-FAIL ");
    out->println(push->error().message());
    return false;
  } else {
    out->println("+OK");
    return true;
  }
}

}  // modem
}  // firebase
