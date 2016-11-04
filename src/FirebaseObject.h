//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef FIREBASE_OBJECT_H
#define FIREBASE_OBJECT_H

#define ARDUINOJSON_USE_ARDUINO_STRING 1
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

#ifndef FIREBASE_JSONBUFFER_SIZE
#define FIREBASE_JSONBUFFER_SIZE JSON_OBJECT_SIZE(32)
#endif // FIREBASE_JSONBUFFER_SIZE

/**
 * Represents value stored in firebase, may be a singular value (leaf node) or
 * a tree structure.
 */
class FirebaseObject {
 public:
  /**
   * Construct from json.
   * \param data JSON formatted string.
   */
  FirebaseObject(const char* data);

  /**
   * Return the value as a boolean.
   * \param optional path in the JSON object.
   * \return result as a bool.
   */
  bool getBool(const String& path = "") const;

  /**
   * Return the value as an int.
   * \param optional path in the JSON object.
   * \return result as an integer.
   */
  int getInt(const String& path = "") const;

  /**
   * Return the value as a float.
   * \param optional path in the JSON object.
   * \return result as a float.
   */
  float getFloat(const String& path = "") const;

  /**
   * Return the value as a String.
   * \param optional path in the JSON object.
   * \return result as a String.
   */
  String getString(const String& path = "") const;

  /**
   * Return the value as a JsonVariant.
   * \param optional path in the JSON object.
   * \return result as a JsonVariant.
   */
  JsonVariant getJsonVariant(const String& path = "") const;


  /**
   *
   * \return Whether there was an error decoding or accessing the JSON object.
   */
  bool success() const;

  /**
   *
   * \return Whether there was an error decoding or accessing the JSON object.
   */
  bool failed() const;

  /**
   *
   * \return Error message if failed() is true.
   */
  const String& error() const;

 private:
  String data_;
  StaticJsonBuffer<FIREBASE_JSONBUFFER_SIZE> buffer_;
  JsonVariant json_;
  mutable String error_;
};

#endif // FIREBASE_OBJECT_H
