#include <string>
// This is needed to compile std::string on esp8266.
template class std::basic_string<char>;

#include "modem/SerialTransceiver.h"
// Bring them into the base namespace for easier use in arduino ide.
using firebase::modem::SerialTransceiver;
