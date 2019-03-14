
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
//A sample that control devices (in this example 2 devices)
// through Firebase Database
//
// A suggested setup for testing this example would be to connect a board 
// with integrated usb and open a serial monitor to see debug messages. 
// Then connect devices on pin D0 and D1 

/*Database Structure
 * 
 * example.firebaseio.com
 *           |
 *           devices
 *                |
 *                DEV1 : 0   // 0 OFF, 1 ON
 *                DEV2 : 0
 */

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
SoftwareSerial esp(D5,D6); // rx,tx




// Set these to run example.
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define device1 D0
#define device2 D1




//Device 1
int currentState1;
int dev1=-1;

//Device2
int currentState2;
int dev2=-1;



void setup() {
  
  
  Serial.begin(9600);
  esp.begin(9600);

  //PinModes
  pinMode(device1,OUTPUT);
  pinMode(device2,OUTPUT);
  
  
  // Connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());


  Serial.println("Setting up ....");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.stream("/devices/");
  delay(1000);
  currentState1 = Firebase.getInt("/devices/DEV1");
  currentState2 = Firebase.getInt("/devices/DEV2");
  digitalWrite(device1,currentState1);
  digitalWrite(device2,currentState2);
}


//LOOP
void loop() {

  //detect error
  detectError();


  //Streaming
   if (Firebase.available()) {
    
     FirebaseObject event = Firebase.readEvent();
     String eventType = event.getString("type");
     String eventPath = event.getString("path");
     int eventData = event.getInt("data");
     Serial.print("event : ");
     Serial.println(eventType);
     Serial.println(eventPath);
     Serial.println(eventData);

      if(!eventPath.equals("/") && eventType.equals("put"))
      controlDevice(eventPath, eventData);
     
  }   

}



//sendDeviceData
void sendDeviceData(int a, int b){

   Serial.println(a);
   Serial.println(b);
    esp.print(a);
    esp.print(b);
    esp.println();

}




//controlDevice
void controlDevice(String deviceName,int eventData){


  int controlDev;
  if(deviceName.equals("/DEV1")){  
             controlDev = device1;
             currentState1=eventData;

  }
  else if(deviceName.equals("/DEV2")){ 
             controlDev = device2;
             currentState2=eventData;
  }        


  //Control the device according to data
  eventData? turnOn(controlDev): turnOff(controlDev);

  //Sends the device state through Rx and in Serial Monitor
  sendDeviceData(currentState1, currentState2);
  delay(30);

}




//Detects Error
void detectError(){

  if (Firebase.failed()) {
    Serial.println("Streaming error");
    Serial.println(Firebase.error());
  }
}



//turn On
void turnOn(int pin){
   digitalWrite(pin,HIGH);
}


//Turn off
void turnOff(int pin){
   digitalWrite(pin,LOW);
}
