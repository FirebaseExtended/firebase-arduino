#include "modem/db/commands.h"

namespace firebase {
namespace modem {

bool StreamCommand::execute(const String& command,
                            InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "BEGIN_STREAM") {
    return false;
  }

  String path = in->readLine().c_str();
  fbase().stream(path);  

  if (fbase().error().length() != 0) {
    out->print("-FAIL ");
    out->println(fbase().error().c_str());
    return false;
  }
  bool running = true;
  DynamicJsonBuffer buffer;
  while(running) {
    if (fbase().available()) {
      FirebaseObject event = fbase().readEvent();
      out->print("+");
      out->print(event.getString("type").c_str());
      out->print(" ");
      String data = event.getString("data");
      out->println(event.getString("path"));
      out->println(data.length());
      out->println(data);
    } else if (in->available()) {
      String command = in->readLine();
      if (command == "END_STREAM") {
        out->println("+OK");
        running = false;
      } else {
        out->println("-FAIL Cannot call any command but END_STREAM.");
      }
    }
  }

  return true;
}

}  // modem
}  // firebase
