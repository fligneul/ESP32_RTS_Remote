#include "RemoteHandler.h"

#include <ArduinoJson.h>
#include <NVSRollingCodeStorage.h>
#include <Preferences.h>

#include "CommandQueue.h"
#include "Constants.h"
#include "RemoteConfig.h"

// Map of all configured remotes
std::map<uint32_t, SomfyRemote *> remoteMap = {};

// Init remotes configuration
void RemoteHandler_::begin() {
    // Get saved remotes
    Preferences preferences;

    preferences.begin(NVS_REMOTE_NAMESPACE, true);

    DynamicJsonDocument config(1024);
    DeserializationError error = deserializeJson(config, preferences.getString("config", "{}").c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    remoteMap.clear();
    for (JsonObject remote : config["remotes"].as<JsonArray>()) {
        uint32_t remoteId = remote["id"];
        String *remoteKey = new String(NVS_REMOTE_KEY_PREFIX + String(remoteId));
        Serial.print("Remote ");
        Serial.println(*remoteKey);

        NVSRollingCodeStorage *rollingCodeStorage = new NVSRollingCodeStorage(NVS_REMOTE_NAMESPACE, remoteKey->c_str());
        SomfyRemote *somfyRemote = new SomfyRemote(GPIO_EMITTER_PIN, remoteId, rollingCodeStorage);
        somfyRemote->setup();

        remoteMap.insert({remoteId, somfyRemote});
    }

    config.clear();
    preferences.end();
}

std::map<uint32_t, SomfyRemote *> *RemoteHandler_::getRemoteMap() { return &remoteMap; }

// Singleton methods

RemoteHandler_ &RemoteHandler_::getInstance() {
    static RemoteHandler_ instance;
    return instance;
}

RemoteHandler_ &RemoteHandler = RemoteHandler.getInstance();
