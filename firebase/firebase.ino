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

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>

const char* host = "burning-heat-7044.firebaseio.com";
const int httpsPort = 443;
const char* fingerprint = "C1 56 CD D8 49 A3 7D D2 1D 49 60 7E 0D 59 A7 7C C1 0E 58 D2";
struct Credential {
  char token[148];
  char ssid[32];
  char password[32];
} cred;
const int maxWifiConnectAttemps = 20;
WiFiServer telnetServer(23);
WiFiClient telnetClient;
String buffer;

struct MultiSerial {
  MultiSerial(HardwareSerial& serial, WiFiClient& telnetClient) : _serial(serial), _telnetClient(telnetClient) {
    _current = &serial;
  }
  HardwareSerial& _serial;
  WiFiClient& _telnetClient;
  Stream *_current;
  
  int available() {
    int serialAvailable = _serial.available();
    if (serialAvailable > 0) {
      _current = &_serial;
      return serialAvailable;
    }
    if (_telnetClient.connected()) {
      int telnetAvailable = _telnetClient.available();
      if (telnetAvailable > 0) {
        _current = &_telnetClient;
        return telnetAvailable;
      }
    }
    return 0;
  }
  String readStringUntil(char c) {
    return _current->readStringUntil(c);
  }
  void println() {
    _current->println();
  }
  void print(const char* str) {
    _current->print(str);
  }
  template<typename T>
  void println(T arg) {
    _current->println(arg);
  }
} WSerial{Serial, telnetClient};



void setup() {
  Serial.begin(115200);
  
  EEPROM.begin(4096);
  EEPROM.get(0, cred);
  WSerial.println();
  WSerial.println("!I #!/dev/fail");
  WSerial.print("!I token: ");
  WSerial.println(cred.token);
  bool connected = false;
  if (strlen(cred.ssid) > 0) {
    connected = wifiConnect();
  }
  if (!connected) {
    WSerial.println("!E no wifi connection");
  }
  softAP("FAIL");
}

bool wifiConnect() {
  WSerial.println("!I connecting to wifi");
  WSerial.print("!I ssid: ");
  WSerial.println(cred.ssid);
  WSerial.print("!I ");
  WiFi.begin(cred.ssid, cred.password);
  for (int i = 0; i < maxWifiConnectAttemps; i++) {
    delay(500);
    WSerial.print(".");
    if (WiFi.status() == WL_CONNECTED) {
      WSerial.println("");
      WSerial.println("!I wifi connected");
      WSerial.print("!I ip: ");
      WSerial.println(WiFi.localIP());
      return true;
    }
  }
  WSerial.println("");
  WSerial.println("!E wifi connection failed");
  return false;
}

void softAP(String ssid) {
    WSerial.println("!I starting soft AP");
    WSerial.print("!I ssid: ");
    WSerial.println(ssid.c_str());
    WiFi.softAP(ssid.c_str());
    WSerial.print("!I ip: ");
    WSerial.println(WiFi.softAPIP());
    telnetServer.begin();
    telnetServer.setNoDelay(true);
    WSerial.println("!I telnet serial started");
    WSerial.println("!I port: 23");   
}

String buildRequest(String data) {
  return data + "?auth=" + cred.token + " HTTP/1.1\r\n" +
       "Host: " + host + "\r\n" +
       "User-Agent: Chiplet\r\n" +
       "Connection: close\r\n";
}

String unquote(String s) {
  if (s[0] == '"' && s[s.length()-1] == '"') {
    return s.substring(1, s.length()-1);
  }
  return s;
}

void loop() {
  
  if (telnetServer.hasClient()) {
    if (telnetClient.connected()) {
      telnetClient.stop();
    }
    telnetClient = telnetServer.available();
  }
  String request;
  if (WSerial.available() > 0) {
    String data = WSerial.readStringUntil('\n');
    data.trim();    
    if (data.startsWith("GET")) {
       WSerial.print("!I request: ");
       WSerial.println(data);
       request = buildRequest(data) + "\r\n";
    } else if (data.startsWith("POST")) {
       WSerial.print("!I request: ");
       WSerial.println(data);
       WSerial.println("!I waiting for payload");
       buffer = buildRequest(data);
    } else if (buffer.startsWith("POST")) {
       // send POST request when complete
       WSerial.print("!I payload: ");
       WSerial.println(data);
       WSerial.print("!I content-length: ");
       WSerial.println(data.length());
       request = buffer + "Content-Length: " + data.length() + "\r\n\r\n" + data;
       buffer = "";
    } else if (data.startsWith("TOKEN")) {
       data.substring(6).toCharArray(cred.token, sizeof(cred.token));
       EEPROM.put(0, cred);
       EEPROM.commit();
       WSerial.print("!I token updated: ");
       WSerial.println(cred.token);
    } else if (data.startsWith("AT+CWJAP=")) {
      String ssidPwd = data.substring(9);
      int i = ssidPwd.indexOf(",");
      unquote(ssidPwd.substring(0, i)).toCharArray(cred.ssid, sizeof(cred.token));
      if (i != -1) {
        unquote(ssidPwd.substring(i+1)).toCharArray(cred.password, sizeof(cred.password));
      } else {
        cred.password[0] = '\0';
      }
      if (wifiConnect()) {
       EEPROM.put(0, cred);
       EEPROM.commit();
       WSerial.println("OK");
      }
    } else if (data.startsWith("AT+CWSAP=")) {
      String ssid = unquote(data.substring(9));
      softAP(ssid);
    } else {
       WSerial.println("!E usage:");
       WSerial.println("!E GET /path");
       WSerial.println("!E POST /path");
       WSerial.println("!E      payload");
       WSerial.println("!E TOKEN token");
       WSerial.println("!E AT+CWJAP=ssid,passwd");
       WSerial.println("!E AT+CWSAP=ssid");
    }
  }

  if (request.length() > 0) {
    WSerial.println("!I request");
    WiFiClientSecure httpClient;
    if (!httpClient.connect(host, httpsPort)) {
      WSerial.println("!E connection failed");
      return;
    }
    if (!httpClient.verify(fingerprint, host)) {
      WSerial.println("!E certificate doesn't match");
      return;
    }
    WSerial.println("!I connected");
    httpClient.print(request);
    request.trim();
    request.replace("\r\n", "\r\n!> ");
    WSerial.print("!> ");
    WSerial.println(request);
    WSerial.println("!I headers");
    while (httpClient.connected()) { 
       String data = httpClient.readStringUntil('\n');
       WSerial.print("!< ");
       WSerial.println(data);
    }
    WSerial.println("!I body");
    while(httpClient.available() > 0) {
      String data = httpClient.readStringUntil('\n');
      WSerial.println(data);
    }
    WSerial.println("!I end of request");
  }
}
