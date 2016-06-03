#ifndef MODEM_JSON_UTIL_H
#define MODEM_JSON_UTIL_H

namespace firebase {
namespace modem {
namespace {
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}
}

// TODO(edcoyne): We should use a json library to escape.
inline std::string EncodeForJson(std::string input) {
  input = ReplaceAll(input, "\\", "\\\\");
  input = ReplaceAll(input, "\"", "\\\"");
  return "\"" + input + "\"";
}

}  // modem
}  // firebase

#endif  // MODEM_JSON_UTIL_H
