//
// Copyright 2016 Google Inc.
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

#ifndef FIREBASE_ARDUINO_H
#define FIREBASE_ARDUINO_H

#include <string>

#include "Firebase.h"
#include "FirebaseObject.h"

/**
 * Main class for Arduino clients to interact with Firebase.
 * This implementation is designed to follow Arduino best practices and favor
 * simplicity over all else.
 * For more complicated usecases and more control see the Firebase class in
 * Firebase.h.
 */
class FirebaseArduino {
 public:
  /**
   * Must be called first. This initialize the client with the given
   * firebase host and credentials.
   * \param host Your firebase db host, usually X.firebaseio.com.
   * \param auth Optional credentials for the db, a secret or token.
   */
  void begin(const String& host, const String& auth = "");

  /**
   * Appends the integer value to the node at path.
   * Equivalent to the REST API's POST.
   * You should check success() after calling.
   * \param path The path of the parent node.
   * \param value Integer value that you wish to append to the node.
   * \return The unique key of the new child node.
   */
  String pushInt(const String& path, int value);

  /**
   * Appends the float value to the node at path.
   * Equivalent to the REST API's POST.
   * You should check success() after calling.
   * \param path The path of the parent node.
   * \param value Float value that you wish to append to the node.
   * \return The unique key of the new child node.
   */
  String pushFloat(const String& path, float value);

  /**
   * Appends the bool value to the node at path.
   * Equivalent to the REST API's POST.
   * You should check success() after calling.
   * \param path The path of the parent node.
   * \param value Bool value that you wish to append to the node.
   * \return The unique key of the new child node.
   */
  String pushBool(const String& path, bool value);

  /**
   * Appends the String value to the node at path.
   * Equivalent to the REST API's POST.
   * You should check success() after calling.
   * \param path The path of the parent node.
   * \param value String value that you wish to append to the node.
   * \return The unique key of the new child node.
   */
  String pushString(const String& path, const String& value);

  /**
   * Appends the JSON data to the node at path.
   * Equivalent to the REST API's POST.
   * You should check success() after calling.
   * \param path The path of the parent node.
   * \param value JSON data that you wish to append to the node.
   * \return The unique key of the new child node.
   */
  String push(const String& path, const JsonVariant& value);

  /**
   * Writes the integer value to the node located at path equivalent to the
   * REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value Integer value that you wish to write.
   */
  void setInt(const String& path, int value);

  /**
   * Writes the float value to the node located at path equivalent to the
   * REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value Float value that you wish to write.
   */
  void setFloat(const String& path, float value);

  /**
   * Writes the bool value to the node located at path equivalent to the
   * REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value Bool value that you wish to write.
   */
  void setBool(const String& path, bool value);

  /**
   * Writes the String value to the node located at path equivalent to the
   * REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value String value that you wish to write.
   */
  void setString(const String& path, const String& value);

  /**
   * Writes the JSON data to the node located at path.
   * Equivalent to the REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value JSON data that you wish to write.
   */
  void set(const String& path, const JsonVariant& value);


  /**
   * Gets the integer value located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to retrieve.
   * \return The integer value located at that path. Will only be populated if success() is true.
   */
  int getInt(const String& path);

  /**
   * Gets the float value located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to retrieve.
   * \return The float value located at that path. Will only be populated if success() is true.
   */
  float getFloat(const String& path);

  /**
   * Gets the string value located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to retrieve.
   * \return The string value located at that path. Will only be populated if success() is true.
   */
  String getString(const String& path);

  /**
   * Gets the boolean value located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to retrieve.
   * \return The boolean value located at that path. Will only be populated if success() is true.
   */
  bool getBool(const String& path);

  /**
   * Gets the json object value located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to retrieve.
   * \return a FirebaseObject value located at that path. Will only be populated if success() is true.
   */
  FirebaseObject get(const String& path);

  /**
   * Remove the node, and possibly entire tree, located at path.
   * You should check success() after calling.
   * \param path The path to the node you wish to remove,
   * including all of its children.
   */
  void remove(const String& path);

  /**
   * Starts streaming any changes made to the node located at path, including
   * any of its children.
   * You should check success() after calling.
   * This changes the state of this object. Once this is called you may start
   * monitoring available() and calling readEvent() to get new events.
   * WARNING: Currently you cannot make another call while the stream is
   * running, otherwise you will crash due to memory issues. See:
   * https://github.com/googlesamples/firebase-arduino/issues/48
   * \param path The path inside of your db to the node you wish to monitor.
   */
  void stream(const String& path);

  /**
   * Checks if there are new events available. This is only meaningful once
   * stream() has been called.
   * \return If a new event is ready.
   */
  bool available();

  /**
   * Reads the next event in a stream. This is only meaningful once stream() has
   * been called.
   * \return FirebaseObject will have ["type"] that describes the event type, ["path"]
   * that describes the effected path and ["data"] that was updated.
   */
  FirebaseObject readEvent();

  /**
   * \return Whether the last command was successful.
   */
  bool success();

  /**
   * \return Whether the last command failed.
   */
  bool failed();

  /**
   * \return Error message from last command if failed() is true.
   */
  const String& error();
 private:
  std::string host_;
  std::string auth_;
  FirebaseError error_;
  std::unique_ptr<FirebaseHttpClient> http_;
};

extern FirebaseArduino Firebase;

#endif // FIREBASE_ARDUINO_H
