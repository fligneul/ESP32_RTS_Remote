#include "RemoteAPI.h"

#include <ArduinoJson.h>
#include <Preferences.h>
#include <SPIFFS.h>

#include "RemoteConfig.h"
#include "RemoteHandler.h"

std::map<uint32_t, Remote> RemoteAPI::readConfig() {
    std::map<uint32_t, Remote> remoteMap = {};
    // Get saved remotes
    Preferences preferences;
    preferences.begin(NVS_REMOTE_NAMESPACE, true);

    DynamicJsonDocument config(1024);
    DeserializationError error = deserializeJson(config, preferences.getString("config", "{}").c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        preferences.end();
        return remoteMap;
    }
    preferences.end();

    for (JsonObject remote : config["remotes"].as<JsonArray>()) {
        uint32_t remoteId = remote["id"];
        preferences.begin(NVS_REMOTE_NAMESPACE, true);
        remoteMap.insert({remoteId, {remoteId, remote["name"], preferences.getUShort(String(NVS_REMOTE_KEY_PREFIX + String(remoteId)).c_str())}});
        preferences.end();
    }

    config.clear();
    return remoteMap;
}

RemoteAPI::RemoteAPI(AsyncWebServer *webServer) : webServer(webServer) {}

void RemoteAPI::begin() {
    // Remote config page
    webServer->on("/api/remotes", HTTP_GET, [=](AsyncWebServerRequest *request) {
        if (request->hasParam("remoteId")) {
            uint32_t remoteId = (uint32_t)request->getParam("remoteId")->value().c_str();
            std::map<uint32_t, Remote> remotesMap = readConfig();
            auto search = remotesMap.find(remoteId);
            if (search != remotesMap.end()) {
                // Build remote json
                AsyncResponseStream *response = request->beginResponseStream("application/json");
                DynamicJsonDocument remote(256);
                remote["id"] = search->second.id;
                remote["name"] = search->second.name;
                remote["rollingCode"] = search->second.rollingCode;

                serializeJson(remote, *response);
                request->send(response);
            } else {
                request->send(404);
            }
        } else {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            DynamicJsonDocument remotesJson(1024);
            for (const auto &remote : readConfig()) {
                // Build remote json
                DynamicJsonDocument remoteJson(256);
                remoteJson["id"] = remote.second.id;
                remoteJson["name"] = remote.second.name;
                remoteJson["rollingCode"] = remote.second.rollingCode;
                remotesJson.add(remoteJson);
                remoteJson.clear();
            }
            serializeJson(remotesJson, *response);
            request->send(response);
        }
    });

    webServer->on("/api/remotes", HTTP_DELETE, [=](AsyncWebServerRequest *request) {
        if (request->hasParam("remoteId")) {
            uint32_t remoteId = (uint32_t)request->getParam("remoteId")->value().c_str();
            std::map<uint32_t, Remote> remotesMap = readConfig();
            auto search = remotesMap.find(remoteId);
            if (search != remotesMap.end()) {
                Serial.print("Delete remote " + String(remoteId, HEX));
                request->send(200);
            } else {
                request->send(404);
            }
        } else {
            request->send(400);
        }
    });

    // Remote config post handler
    webServer->on("/api/remotes", HTTP_POST, [=](AsyncWebServerRequest *request) {
        Preferences preferences;
        preferences.begin(NVS_REMOTE_NAMESPACE, true);

        int params = request->params();
        for (int i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->isPost()) {
            }
        }
        preferences.end();
        request->redirect("/saved");
        delay(3000);
        ESP.restart();
    });
}