#include "thing/Portal.h"
#include <ArduinoJson.h>

namespace thing {

Portal::Portal()
    : callback_([](const Config&){}),
      debug_([](const char*){}) {}

void Portal::SetDebugHandler(std::function<void(const char* message)> handler) {
  debug_ = std::move(handler);
}

void Portal::Start(const Config& config) {
  config_ = config;
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
      function scan() {
        document.getElementById('scanning').style.display='inline';
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/wifi/scan", true);
        xhr.onreadystatechange = function () {
          if (xhr.readyState==4 && xhr.status==200) {
           load_networks(JSON.parse(xhr.responseText));
           document.getElementById('scanning').style.display='none';
          }
        }
        xhr.send();
      }
      function load_networks(networks) {
        select = document.getElementById('networks');
        select.innerHtml = '';
        networks.sort(function(a, b) {
          return b.rssi - a.rssi;
        });
        networks.forEach(function(network) {
          option = document.createElement('option');
          option.value = network.ssid;
          option.text = network.ssid + ' (' + network.rssi +  ')';
          select.add(option);
        });
        select.style.display='inline';
      }
      function set_network(select) {
        document.getElementById('ssid').value = select[select.selectedIndex].value;
      }
      </script>
      </head>
      <body>
      <div>Host: <input id='host'></div>
      <div>Auth: <input id='auth'></div>
      <div>Path: <input id='path'></div>
      <div>Wifi SSID: <input id='ssid'><button onclick='scan();'>scan</button></div>
      <div id='scanning' style='display:none'>Scanning...</div>
      <div><select size=10 id='networks' style='display:none' onchange='set_network(this);'></select></div>
      <div>Wifi Key: <input id='key'></div>
      <div><button onclick='send_config();'>Save</button></div>
      <div id='loading'>Loading....</div>
      <div id='saving' style='display:none'>Saving....</div>
      <script>fetch_config();</script>
      </body>
    )";
    static const PROGMEM char type[] = "text/html";

    server_.send_P(200, type, page);
    debug_("served root page.");
  });

  server_.on("/config", [&] () {
    if (server_.method() == HTTP_GET) {
      auto client = server_.client();

      ConfigJsonSerializer serializer(config_);
      server_.setContentLength(serializer.content_length());
      server_.send(200, "application/json");
      serializer.SerializeTo(&client);

      debug_("config retrieved");
    } else if (server_.method() == HTTP_POST) {
      DynamicJsonBuffer jsonBuffer;
      if (!server_.hasArg("config")) {
        server_.send(500, "text/plain", "Missing config.\r\n");
        debug_("Config updated called without param.");
        return;
      }

      char* buffer;
      { // Scoped to free String memory.
        String config = server_.arg("config");
        buffer = (char*)malloc(config.length()+1);
        memcpy(buffer, config.c_str(), config.length()+1);
      }
      { // Scoped because serializer is invalid after free().
        ConfigJsonSerializer serializer(buffer);
        serializer.DeserializeTo(&config_);
        free(buffer);
      }

      callback_(config_);
      server_.send(200, "text/plain", "");
      debug_("config updated.");
    }
  });

  server_.on("/wifi/scan", [&] () {
    int net_count = WiFi.scanNetworks();
    DynamicJsonBuffer json_buffer;
    JsonArray& data = json_buffer.createArray();
    for (int i=0; i < net_count; i++) {
      JsonObject& entry = data.createNestedObject();
      entry["ssid"] = WiFi.SSID(i);
      entry["rssi"] = WiFi.RSSI(i);
    }
    // Free station info from memory.
    WiFi.scanDelete();

    String buffer;
    data.printTo(buffer);
    server_.send(200, "application/json", buffer);
    debug_("served networks.");
  });

  server_.begin();
  debug_("Portal started.");
}

void Portal::Loop() {
  server_.handleClient();
}

void Portal::NotifyOnUpdate(std::function<void(const Config& config)> callback) {
  callback_ = callback;
}

};
