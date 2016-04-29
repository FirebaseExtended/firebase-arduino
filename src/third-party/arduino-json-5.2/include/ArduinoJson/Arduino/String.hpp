// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifndef ARDUINO

#ifndef ARDUINO_STRING_OVERRIDE
#define ARDUINO_STRING_OVERRIDE
#include <string>
typedef std::string String;
#endif

#else

#include <WString.h>

#endif
