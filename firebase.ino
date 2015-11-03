#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>

const char* ssid = "GoogleGuest";
const char* password = "";

const char* host = "burning-heat-7044.firebaseio.com";
const int httpsPort = 443;
const char* fingerprint = "C1 56 CD D8 49 A3 7D D2 1D 49 60 7E 0D 59 A7 7C C1 0E 58 D2";
struct Credential {
  char token[148];
} cred;

String buffer;

void setup() {
  Serial.begin(115200);
  
  EEPROM.begin(512);
  EEPROM.get(0, cred);
  Serial.println();
  Serial.println("!I #!/dev/chiplet");
  Serial.print("!I token: ");
  Serial.println(cred.token);
  Serial.print("!I connecting to wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("!I wifi connected: ");
  Serial.println(WiFi.localIP());
}

String buildRequest(String data) {
  return data + "?auth=" + cred.token + " HTTP/1.1\r\n" +
       "Host: " + host + "\r\n" +
       "User-Agent: Chiplet\r\n" +
       "Connection: close\r\n";
}

void loop() {
  String request;
  
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    
    if (data.startsWith("GET")) {
       Serial.println(String("!I request: ") + data);
       request = buildRequest(data) + "\r\n";
    } else if (data.startsWith("POST")) {
       Serial.println(String("!I request: ") + data);
       Serial.println("!I waiting for payload");
       buffer = buildRequest(data);
    } else if (buffer.startsWith("POST")) {
       // send POST request when complete
       Serial.println(String("!I payload: ") + data);
       Serial.println(String("!I content-length: ") + data.length());
       request = buffer + "Content-Length: " + data.length() + "\r\n\r\n" + data;
       buffer = "";
    } else if (data.startsWith("TOKEN")) {
       data.substring(6).toCharArray(cred.token, sizeof(cred.token));
       EEPROM.put(0, cred);
       EEPROM.commit();
       Serial.println(String("!I token updated: ") + cred.token);
    } else {
       Serial.println("!E usage:");
       Serial.println("!E GET /path");
       Serial.println("!E POST /path");
       Serial.println("!E      payload");
       Serial.println("!E TOKEN token");
    }
  }

  if (request.length() > 0) {
    WiFiClientSecure client;
    if (!client.connect(host, httpsPort)) {
      Serial.println("!E connection failed");
      return;
    }
    if (!client.verify(fingerprint, host)) {
      Serial.println("!E certificate doesn't match");
      return;
    }
    Serial.println("!I connected");
    client.print(request);
    request.trim();
    request.replace("\r\n", "\r\n!> ");
    Serial.println(String("!> ") + request);
    Serial.println("!I headers");
    while (client.connected()) { 
       String data = client.readStringUntil('\n');
       Serial.println("!< " + data);
    }
    Serial.println("!I body");
    while(client.available() > 0) {
      String data = client.readStringUntil('\n');
      Serial.println(data);
    }
  }
}
