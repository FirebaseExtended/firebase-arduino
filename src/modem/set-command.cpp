#include "modem/commands.h"
#include "modem/json_util.h"

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

  std::unique_ptr<FirebaseSet> set(fbase().setPtr(path.c_str(),
                                                  EncodeForJson(data).c_str()));

  if (set->error()) {
    out->print("-FAIL ");
    out->println(set->error().message().c_str());
    return false;
  } else {
    out->println("+OK");
    return true;
  }
}

}  // modem
}  // firebase
