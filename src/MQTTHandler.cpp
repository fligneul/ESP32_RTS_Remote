#include "MQTTHandler.h"

#include <MQTT.h>
#include <SPIFFS.h>
#include <WiFi.h>

#include <regex>

#include "CommandQueue.h"
#include "Constants.h"
#include "MQTTConfig.h"
#include "RemoteConfig.h"
#include "RemoteCommand.h"
#include "WifiHandler.h"

#define RECONNECT_INTERVAL 10000

WiFiClient wifiClient;
MQTTClient mqttClient;

unsigned long lastTry = 0;

void handleMessage(String &topic, String &payload) {
    Serial.println("Received MQTT frame: " + topic + " - " + payload);
    std::string s_topic = topic.c_str();

    char str[64];
    sprintf(str, "%s%s%s", "/.*/", NVS_REMOTE_KEY_PREFIX, "([A-F0-9]+)/cmd$");
    std::smatch matches;

    if (std::regex_match(s_topic, matches, std::regex(str))) {
        RemoteCommand command = {
            (uint32_t)strtoul(matches.str(1).c_str(), 0, 16), payload};
        CommandQueue.registerCommand(command);
    } else {
        Serial.println("MQTT topic not supported");
    }
}

void MQTTHandler::begin() {
    preferences.begin(NVS_MQTT_NAMESPACE, true);
    isConfigured = WifiHandler.configured() &&
                   preferences.isKey(NVS_MQTT_HOST_KEY) &&
                   !preferences.getString(NVS_MQTT_HOST_KEY).isEmpty();

    if (isConfigured) {
        mqttClient.begin(preferences.getString(NVS_MQTT_HOST_KEY).c_str(),
                         wifiClient);
        mqttClient.onMessage(handleMessage);

        connect();
    }
}

void MQTTHandler::loop() {
    if (isConfigured && WifiHandler.connected()) {
        if (!mqttClient.connected()) {
            connect();
        } else {
            mqttClient.loop();
        }
    }
}

void MQTTHandler::connect() {
    // Async connect to MQTT
    if (WifiHandler.connected() && !mqttClient.connected() &&
        ((millis() - lastTry) > RECONNECT_INTERVAL)) {
        lastTry = millis();

        if (mqttClient.connect(
                preferences
                    .getString(NVS_MQTT_CLIENT_NAME_KEY,
                               NVS_MQTT_CLIENT_NAME_DEFAULT)
                    .c_str(),
                preferences.getString(NVS_MQTT_USER_KEY).c_str(),
                preferences.getString(NVS_MQTT_PASSWORD_KEY).c_str())) {
            Serial.println("MQTT Connected");
            // add config
            mqttClient.subscribe(
                preferences.getString(NVS_MQTT_TOPIC_KEY,
                                      NVS_MQTT_TOPIC_DEFAULT) +
                WILDCARD_TOPIC);
        }
    }
}
