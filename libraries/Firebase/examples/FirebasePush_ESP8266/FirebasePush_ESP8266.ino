#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Firebase.h>

WiFiClientSecure wifiClient;

int pushButtonPin = 1;
int previousState = 1;

void setup() {
    pinMode(pushButtonPin, INPUT);
    digitalWrite(pushButtonPin, HIGH);
 
    Serial.begin(9600);

    WiFi.begin("my-ssid", "my-password");
    Firebase.begin(wifiClient, "my-firebase.firebaseio.com");
    Firebase.auth("my-firebase-token-or-secrent");
}

void loop() {
  int buttonState = digitalRead(pushButtonPin);
  if (buttonState == 0 && buttonState != previousState) {
    String record = Firebase.child("hits").push("{\".sv\": \"timestamp\"}");
    Serial.print(record);
  }
  previousState = buttonState;
}
