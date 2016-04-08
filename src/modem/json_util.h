#ifndef MODEM_JSON_UTIL_H
#define MODEM_JSON_UTIL_H

namespace firebase {
namespace modem {

// TODO(edcoyne): We should use a json library to escape.
inline String EncodeForJson(String input) {
  input.replace("\\", "\\\\");
  input.replace("\"", "\\\"");
  return "\"" + input + "\"";
}

}  // modem
}  // firebase

#endif  // MODEM_JSON_UTIL_H
