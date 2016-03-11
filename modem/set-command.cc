#include "modem/commands.h"

namespace firebase {
namespace modem {

bool SetCommand::execute(const String& command,
                         InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "SET") {
    return false;
  }

  String path(in->readStringUntil(' '));
  String data(in->readLine());

  // First char will be a ' ', drop it. Last two chars will be \r\n, drop them.
  data = data.substring(1, data.length() - 2);

  // TODO(ed7coyne): encode data as json.

  std::unique_ptr<FirebaseSet> set(fbase().setPtr(path, data));

  if (set->error()) {
    out->print("-FAIL ");
    out->println(set->error().message());
    return false;
  } else {
    out->println("+OK");
    return true;
  }
}

}  // modem
}  // firebase
