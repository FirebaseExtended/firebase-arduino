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

  std::string path = in->readLine().c_str();
  std::unique_ptr<FirebaseStream> stream(fbase().streamPtr(path));

  if (stream->error()) {
    out->print("-FAIL ");
    out->println(stream->error().message().c_str());
    return false;
  }

  bool running = true;
  DynamicJsonBuffer buffer;
  while(running) {
    if (stream->available()) {
      std::string json;
      FirebaseStream::Event event = stream->read(json);
      out->print("+");
      out->print(FirebaseStream::EventToName(event).c_str());
      out->print(" ");
      const auto& object = buffer.parseObject(json.c_str());
      String data = object["data"];
      out->println(object["path"].asString());
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
