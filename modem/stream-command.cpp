#include "modem/commands.h"

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

  String path = in->readLine();
  std::unique_ptr<FirebaseStream> stream(fbase().streamPtr(path));

  if (stream->error()) {
    out->print("-FAIL ");
    out->println(stream->error().message());
    return false;
  }

  bool running = true;
  while(running) {
    if (stream->available()) {
      String json;
      FirebaseStream::Event event = stream->read(json);
      out->print("+");
      out->print(FirebaseStream::EventToName(event) + " ");
      // TODO(edcoyne): add json parsing and get real path.
      out->println("/dummy/path");
      out->println(json.length());
      out->println(json);
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
