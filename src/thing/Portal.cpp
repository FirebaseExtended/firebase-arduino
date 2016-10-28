#include "thing/Portal.h"
#include "third-party/arduino-json-5.6.7/include/ArduinoJson.h"

namespace thing {

Portal::Portal(const Config& config) : config_(config), callback_([](const Config&){}) {}

void Portal::Start() {
  server_.on("/", [&] () {
    static const PROGMEM char page[] = R"(
      <head>
      <script>
      function fetch_config() {
        document.getElementById('loading').style.display='inline'
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/config", true);
        xhr.onreadystatechange = function () {
          if (xhr.readyState==4 && xhr.status==200) {
           populate_values(JSON.parse(xhr.responseText));
           document.getElementById('loading').style.display='none'
          }
        }
        xhr.send();
      }
      function populate_values(config) {
        document.getElementById('host').value = config.host;
        document.getElementById('auth').value = config.auth;
        document.getElementById('path').value = config.path;
        document.getElementById('ssid').value = config.wifi_ssid;
        document.getElementById('key').value = config.wifi_key;
      }
      function build_config() {
        var config = {
          host : document.getElementById('host').value,
          auth : document.getElementById('auth').value,
          path : document.getElementById('path').value,
          wifi_ssid : document.getElementById('ssid').value,
          wifi_key : document.getElementById('key').value
          };
        return config;
      }
      function send_config() {
        document.getElementById('saving').style.display='inline'
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "/config", true);
        xhr.send("config=" + JSON.stringify(build_config()));
        xhr.onreadystatechange = function () {
          if (xhr.readyState==4 && xhr.status==200) {
           document.getElementById('saving').style.display='none'
          }
        }
      }
      </script>
      </head>
      <body>
      <div>Host: <input id='host'></div>
      <div>Auth: <input id='auth'></div>
      <div>Path: <input id='path'></div>
      <div>Wifi SSID: <input id='ssid'></div>
      <div>Wifi Key: <input id='key'></div>
      <div><button onclick='send_config();'>Save</button></div>
      <div id='loading'>Loading....</div>
      <div id='saving' style='display:none'>Saving....</div>
      <script>fetch_config();</script>
      </body>
    )";
    static const PROGMEM char type[] = "text/html";

    server_.send_P(200, type, page);
  });

  server_.on("/config", [&] () {
    DynamicJsonBuffer jsonBuffer;
    if (server_.method() == HTTP_GET) {
      JsonObject& root = jsonBuffer.createObject();
      root["host"] = config_.host.c_str();
      root["auth"] = config_.auth.c_str();
      root["path"] = config_.path.c_str();
      root["wifi_ssid"] = config_.wifi_ssid.c_str();
      root["wifi_key"] = config_.wifi_key.c_str();

      String buffer;
      root.printTo(buffer);
      server_.send(200, "application/json", buffer);
    } else if (server_.method() == HTTP_POST) {
      if (!server_.hasArg("config")) {
        server_.send(500, "text/plain", "Missing config.\r\n");
        return;
      }
      String config = server_.arg("config");
      JsonObject& root = jsonBuffer.parseObject(config.c_str());
      config_.host = root["host"].asString();
      config_.auth = root["auth"].asString();
      config_.path = root["path"].asString();
      config_.wifi_ssid = root["wifi_ssid"].asString();
      config_.wifi_key = root["wifi_key"].asString();
      callback_(config_);
      server_.send(200, "text/plain", "");
    }
  });
  server_.begin();
}

void Portal::Loop() {
  server_.handleClient();
}

void Portal::NotifyOnUpdate(std::function<void(const Config& config)> callback) {
  callback_ = callback;
}

};
