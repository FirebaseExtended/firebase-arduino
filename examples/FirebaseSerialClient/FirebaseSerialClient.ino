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

// FirebaseSerialClient is a sample that would sit on the other
// end of a serial connection and communicate with our chiplet.
// When you hold down the send button it will generate a random number
// of flashes and store that in Firebase. Then when you hold down the 
// flash button it will read that value from Firebase and flash the
// led the appropriate number of times. If you hold both it will delete
// the stored value.
// This example touches most of the basic calls over the serial interface.

const int sendButtonPin = 4;
const int flashButtonPin = 5;
const int ledPin = 6;

// Set these parameters.
const String network_ssid = "";
const String host = "";
const String auth = "";

bool initialized = false;

void setup() {
  pinMode(sendButtonPin, INPUT);
  pinMode(flashButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Connect to pc over usb.
  Serial.begin(9600);
  
  // Connect to chiplet over the Serial1 port (pins 19 and 18).
  // If you don't have a Serial1 port than remove all debug output and switch to Serial
  Serial1.begin(9600);

  // Note, in production you only need to set NETWORK when your device changes location.
  // It will persist through power cycles.
  dataPrintLn(String("NETWORK ") + network_ssid);
  String response = dataReadLn();
  if (response  != "+CONNECTED") {      
    debugPrintLn("Failed to connect to network: " + response);
    return;
  }
  debugPrintLn("Connected to " + network_ssid);
  
  dataPrintLn(String("INIT ") + host + " " + auth);    
  response = dataReadLn();
  if (response != "+OK") {
    debugPrintLn("Failed to initalize: " + response);
    return;
  }
  initialized = true;    
}

void loop() {
  if (!initialized) {
    return;
  }

  if (digitalRead(sendButtonPin) && digitalRead(flashButtonPin)) {
    dataPrintLn("REMOVE /led_flashes");
    String response = dataReadLn();
    if (response != "+OK") {
      debugPrintLn("Error during REMOVE: " + response);          
    }    
    
  } else if (digitalRead(sendButtonPin)) {
    debugPrintLn("Sending random number.");
    int flashes = random(10);

    dataPrintLn(String("SET /led_flashes") + flashes);
    String response = dataReadLn();
    if (response != "+OK") {
      debugPrintLn("Error during SET: " + response);          
    }

    // Write to a log as well so we can see the history.
    dataPrintLn(String("PUSH /led_flashes_log ") + flashes);
    if (dataReadType() == '-') {
      debugPrintLn("Error during PUSH: " + dataReadLn());      
    } else {
      // Drain input, we don't care about the new key.
      dataReadLn();
    }
    
    debugPrintLn("Done sending random number.");
    
  } else if (digitalRead(flashButtonPin)) {
    debugPrintLn("Flashing LED.");
    dataPrintLn(String("GET# /led_flashes"));
    if (dataReadType() == '-') {
      debugPrintLn("Error during GET: " + dataReadLn());          
    }

    int flashes = atoi(dataReadLn().c_str());
    flashLed(flashes);    
    
    debugPrintLn("Done flashing LED.");
  }
}

void flashLed(int times) {
  for (int i=0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(300);
  }
}

void debugPrintLn(const String& message) {
    Serial.println(message);
}

void dataPrintLn(const String& message) {
    Serial1.println(message);  
}

char dataReadType() {
  return Serial1.read();
}

String dataReadLn() {
  String line = Serial1.readStringUntil('\r');
  // Dump \r\n
  Serial1.read();
  Serial1.read();
  return line;
}

