#include "modem/db/commands.h"

namespace firebase {
namespace modem {

bool BeginCommand::execute(const String& command,
                         InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "BEGIN_DB") {
    return false;
  }

  String host;
  String auth;

  String data(in->readLine());

  int space_index = data.indexOf(' ');
  if (space_index == -1) {
    // host only.
    host = data;
  } else {
    // host and auth.
    host = data.substring(0, space_index);
    auth = data.substring(space_index + 1);
  }

  if (host.length() == 0) {
    out->println("-FAIL Missing host");
    return false;
  }

  new_firebase_.reset(new Firebase(host.c_str(), auth.c_str()));

  out->println("+OK");
  return true;
}

std::unique_ptr<Firebase> BeginCommand::firebase() {
  return std::move(new_firebase_);
}

}  // modem
}  // firebase
