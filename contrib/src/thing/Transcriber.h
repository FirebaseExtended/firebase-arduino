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

#ifndef transcriber_h
#define transcriber_h

#include <Arduino.h>
#include <memory>
#include <utility>
#include <vector>
#include "Firebase.h"
#include "FirebaseObject.h"
#include "thing/Config.h"

namespace thing {

class Transcriber {
 public:
  Transcriber();
  void Setup(const Config& config);
  void UpdateConfig(const Config& config);
  void Loop();

  void SetDebugHandler(std::function<void(const char* message)> handler);

 private:
  void Init(const Config& config);

  void ProcessInitialUpdate(const FirebaseObject& update);
  void ProcessIncrementalUpdate(const FirebaseObject& update);

  // Wrapper around Firebase::Set that stops the stream before setting,
  // Currently we have memory issues if we try to have two simultanious
  // SSL connections.
  void SetValue(const std::string& path, const std::string& value);

  std::unique_ptr<Firebase> fbase_;
  std::unique_ptr<FirebaseStream> stream_;

  int digital_in_ = 0;
  float analog_activation_threshold_ = 0.0f;
  float analog_in_ = 0.0f;

  int pin_digital_out_;
  int pin_digital_in_;
  int pin_analog_out_;
  int pin_analog_in_;

  std::string path_;

  bool connected_ = false;
  std::function<void(const char* message)> debug_;
};

};  // thing


#endif  // transcriber_h
