#include "RemoteAPI.h"

#include <ArduinoJson.h>
#include <Preferences.h>
#include <SPIFFS.h>

#include "RemoteConfig.h"
#include "RemoteHandler.h"

std::map<uint32_t, Remote> RemoteAPI::readConfig() {
    std::map<uint32_t, Remote> remotesMap = {};
    // Get saved remotes
    Preferences preferences;
    preferences.begin(NVS_REMOTE_NAMESPACE, true);

    DynamicJsonDocument config(1024);
    DeserializationError error = deserializeJson(config, preferences.getString("config", "{}").c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        preferences.end();
        return remotesMap;
    }
    preferences.end();

    for (JsonObject remote : config["remotes"].as<JsonArray>()) {
        uint32_t remoteId = remote["id"];
        remotesMap.insert({remoteId, {remoteId, remote["name"]}});
    }

    config.clear();
    return remotesMap;
}

std::map<uint32_t, Remote> RemoteAPI::readConfigWithRollingCode() {
    std::map<uint32_t, Remote> remotesMap = readConfig();

    for (const auto &remote : remotesMap) {
        Preferences preferences;
        preferences.begin(NVS_REMOTE_NAMESPACE, true);
        Remote tempRemote = remote.second;
        tempRemote.rollingCode = preferences.getUShort(String(NVS_REMOTE_KEY_PREFIX + String(remote.first, HEX)).c_str());
        // Update value
        remotesMap[remote.first] = tempRemote;
        preferences.end();
    }

    return remotesMap;
}

void RemoteAPI::addRemote(Remote remote) {
    std::map<uint32_t, Remote> remotesMap = readConfig();

    // Insert new remote in the remotes map
    remotesMap.insert({remote.id, remote});

    // Save new remotes config
    saveConfig(remotesMap);

    // Save rolling code
    Preferences preferences;
    preferences.begin(NVS_REMOTE_NAMESPACE);
    preferences.putUShort(String(NVS_REMOTE_KEY_PREFIX + String(remote.id, HEX)).c_str(), remote.rollingCode);
    preferences.end();
}

bool RemoteAPI::removeRemote(uint32_t remoteId) {
    std::map<uint32_t, Remote> remotesMap = readConfig();

    auto search = remotesMap.find(remoteId);
    if (search != remotesMap.end()) {
        // Remove remote in the remotes map
        remotesMap.erase(remoteId);

        // Save new remotes config
        saveConfig(remotesMap);

        // Remove rolling code
        Preferences preferences;
        preferences.begin(NVS_REMOTE_NAMESPACE);
        preferences.remove(String(NVS_REMOTE_KEY_PREFIX + String(remoteId, HEX)).c_str());
        preferences.end();

        return true;
    } else {
        Serial.println("Error: Remote unknown");
        return false;
    }
}

void RemoteAPI::saveConfig(std::map<uint32_t, Remote> remotesMap) {
    Preferences preferences;
    preferences.begin(NVS_REMOTE_NAMESPACE);

    DynamicJsonDocument configJson(1024);
    JsonArray remotes = configJson.createNestedArray("remotes");

    for (const auto &remote : remotesMap) {
        // Build remote json
        DynamicJsonDocument remoteJson(256);
        remoteJson["id"] = remote.second.id;
        remoteJson["name"] = remote.second.name;
        remotes.add(remoteJson);
        remoteJson.clear();
    }

    String jsonConfig;
    serializeJson(configJson, jsonConfig);
    configJson.clear();

    preferences.putString("config", jsonConfig);
    preferences.end();
}

RemoteAPI::RemoteAPI(AsyncWebServer *webServer) : webServer(webServer) {}

void RemoteAPI::begin() {
    // Remote config page
    webServer->on("/api/remotes", HTTP_GET, [=](AsyncWebServerRequest *request) {
        if (request->hasParam("remoteId")) {
            uint32_t remoteId = (uint32_t)std::stoul(request->getParam("remoteId")->value().c_str());
            std::map<uint32_t, Remote> remotesMap = readConfigWithRollingCode();

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
            for (const auto &remote : readConfigWithRollingCode()) {
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
            uint32_t remoteId = (uint32_t)std::stoul(request->getParam("remoteId")->value().c_str());
            if (removeRemote(remoteId)) {
                Serial.println("Remote " + String(remoteId, HEX) + " deleted");
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
        int params = request->params();
        Remote tmpRemote;
        for (int i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->name() == "remoteId" && !p->value().isEmpty()) {
                tmpRemote.id = (uint32_t)std::stoul(p->value().c_str());
            }
            if (p->name() == "remoteName" && !p->value().isEmpty()) {
                tmpRemote.name = p->value();
            }
            if (p->name() == "rollingCode" && !p->value().isEmpty()) {
                tmpRemote.rollingCode = (uint16_t)std::stoul(p->value().c_str());
            }
        }
        addRemote(tmpRemote);
        request->send(200);
    });
}