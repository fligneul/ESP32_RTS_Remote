#include "RemoteWebConfig.h"

#include <ArduinoJson.h>
#include <Preferences.h>
#include <SPIFFS.h>

#include <map>

#include "RemoteConfig.h"
#include "RemoteHandler.h"

std::map<uint32_t, String> readConfig() {
    std::map<uint32_t, String> remoteMap = {};
    // Get saved remotes
    Preferences preferences;

    preferences.begin(NVS_REMOTE_NAMESPACE, true);

    DynamicJsonDocument config(1024);
    DeserializationError error = deserializeJson(config, preferences.getString("config", "{}").c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
    }

    remoteMap.clear();
    for (JsonObject remote : config["remotes"].as<JsonArray>()) {
        remoteMap.insert({(uint32_t)remote["id"], remote["name"]});
    }
    return remoteMap;
}

String remotesProcessor(const String &var) {
    String processed = String();
    if (var == "REMOTES") {
        for (const auto &kv : readConfig()) {
            processed += "<tr><td>0x" + String(kv.first, HEX) + "</td><td>" + kv.second + "</td><td><a href=\"/remote?remoteId=" + kv.first +
                         "\">Update</a></td><td><a href=\"#\">Delete</a></td></tr>";
        }
    }
    return processed;
};

RemoteWebConfig::RemoteWebConfig(AsyncWebServer *webServer) : webServer(webServer) {}

void RemoteWebConfig::begin() {
    // Remote config page
    webServer->on("/remotes", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/remotes.html", "text/html", false, remotesProcessor); });

    webServer->on("/remote", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/remote.html", "text/html", false); });
}