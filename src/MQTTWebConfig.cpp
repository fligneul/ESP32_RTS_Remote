#include "MQTTWebConfig.h"

#include <Preferences.h>
#include <SPIFFS.h>

#include "MQTTConfig.h"

#define HOST_PARAM "host"
#define CLIENT_PARAM "client"
#define USER_PARAM "user"
#define PASSWD_PARAM "passwd"
#define TOPIC_PARAM "topic"

// Replaces placeholder
String mqttProcessor(const String &var) {
    Preferences preferences;
    preferences.begin(NVS_MQTT_NAMESPACE, true);
    String processed = String();
    if (var == "HOST") {
        processed = preferences.getString(NVS_MQTT_HOST_KEY);
    } else if (var == "CLIENT") {
        processed = preferences.getString(NVS_MQTT_CLIENT_NAME_KEY,
                                          NVS_MQTT_CLIENT_NAME_DEFAULT);
    } else if (var == "USER") {
        processed = preferences.getString(NVS_MQTT_USER_KEY);
    } else if (var == "PASSWD") {
        processed = preferences.getString(NVS_MQTT_PASSWORD_KEY);
    } else if (var == "TOPIC") {
        processed =
            preferences.getString(NVS_MQTT_TOPIC_KEY, NVS_MQTT_TOPIC_DEFAULT);
    }
    preferences.end();
    return processed;
}

MQTTWebConfig::MQTTWebConfig(AsyncWebServer *webServer)
    : webServer(webServer) {}

void MQTTWebConfig::begin() {
    // MQTT config page
    webServer->on("/mqtt", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/mqtt.html", "text/html", false, mqttProcessor);
    });

    // Wifi config post handler
    webServer->on("/mqtt", HTTP_POST, [=](AsyncWebServerRequest *request) {
        Preferences preferences;
        preferences.begin(NVS_MQTT_NAMESPACE, false);

        Serial.println("Post received");

        int params = request->params();
        for (int i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->name() == HOST_PARAM && !p->value().isEmpty()) {
                const char *host = p->value().c_str();
                preferences.putString(NVS_MQTT_HOST_KEY, host);
            }
            if (p->name() == CLIENT_PARAM && !p->value().isEmpty()) {
                const char *client = p->value().c_str();
                preferences.putString(NVS_MQTT_CLIENT_NAME_KEY, client);
            }
            if (p->name() == USER_PARAM && !p->value().isEmpty()) {
                const char *user = p->value().c_str();
                preferences.putString(NVS_MQTT_USER_KEY, user);
            }
            if (p->name() == PASSWD_PARAM && !p->value().isEmpty()) {
                const char *passwd = p->value().c_str();
                preferences.putString(NVS_MQTT_PASSWORD_KEY, passwd);
            }
            if (p->name() == TOPIC_PARAM && !p->value().isEmpty()) {
                const char *topic = p->value().c_str();
                preferences.putString(NVS_MQTT_TOPIC_KEY, topic);
            }
        }
        preferences.end();
        request->redirect("/saved");
        delay(3000);
        ESP.restart();
    });
}
