// FirebasePush_ESP8266 is a sample that write a timestamp to a firebase
// everytime it is powered on.

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Firebase.h>

#define WIFI_SSID "..."
#define WIFI_PASSWORD "..."
#define FIREBASE_HOST "..."
#define FIREBASE_TOKEN "..."

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST);
  Firebase.auth(FIREBASE_TOKEN);

  WiFiClientSecure wifiClient;
  if (!wifiClient.connect(Firebase.host(), Firebase.port())) {
    Serial.print("connected failed");
  }
  if (!wifiClient.verify(Firebase.fingerprint(), Firebase.host())) {
    Serial.println("certificate verification failed");
    return;
  }

  String req = Firebase.child("logs").push("{\".sv\": \"timestamp\"}");
  Serial.println("request:");
  Serial.println(req);
  wifiClient.print(req);

  Serial.println("response:");
  while(wifiClient.connected() || wifiClient.available() > 0) {
    String data = wifiClient.readStringUntil('\n');
    Serial.println(data);
  }  
}

void loop() {
}
