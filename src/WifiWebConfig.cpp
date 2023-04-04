#include "WifiWebConfig.h"

#include <Preferences.h>
#include <WiFi.h>

#include "WifiConfig.h"

#define SSID_PARAM "ssid"
#define PASSWD_PARAM "passwd"
#define HOSTNAME_PARAM "hostname"

// Replaces placeholder
String wifiProcessor(const String &var) {
    Preferences preferences;
    preferences.begin(NVS_WIFI_NAMESPACE, true);
    String processed = String();
    if (var == "SSID") {
        processed = preferences.getString(NVS_WIFI_SSID_KEY);
    } else if (var == "PASSWD") {
        processed = preferences.getString(NVS_WIFI_PASSWORD_KEY);
    } else if (var == "HOSTNAME") {
        processed = preferences.getString(NVS_WIFI_HOSTNAME_KEY, NVS_WIFI_HOSTNAME_KEY_DEFAULT);
    } else if (var == "AVAILABLE_WIFI") {
        for (size_t i = 0; i < WiFi.scanComplete(); i++) {
            processed += "<p><a class=\"ssid-link\" href=\"#\">" + WiFi.SSID(i) + "</a></p>";
        }
    }
    preferences.end();
    return processed;
};

WifiWebConfig::WifiWebConfig(AsyncWebServer *webServer) : webServer(webServer) { WiFi.scanNetworks(true); }

void WifiWebConfig::begin(const bool accessPoint) {
    if (accessPoint) {
        // Web Server Root URL
        webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->redirect("/wifi"); });
    }

    // Wifi config page
    webServer->on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifi.html", "text/html", false, wifiProcessor); });

    // Wifi config post handler
    webServer->on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
        Serial.println("Post received");
        Preferences preferences;
        preferences.begin(NVS_WIFI_NAMESPACE, false);

        int params = request->params();
        for (int i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->isPost()) {
                if (p->name() == SSID_PARAM && !p->value().isEmpty()) {
                    const char *ssid = p->value().c_str();
                    preferences.putString(NVS_WIFI_SSID_KEY, ssid);
                }
                if (p->name() == PASSWD_PARAM && !p->value().isEmpty()) {
                    const char *pass = p->value().c_str();
                    preferences.putString(NVS_WIFI_PASSWORD_KEY, pass);
                }
                if (p->name() == HOSTNAME_PARAM && !p->value().isEmpty()) {
                    const char *hostname = p->value().c_str();
                    preferences.putString(NVS_WIFI_HOSTNAME_KEY, hostname);
                }
            }
        }
        preferences.end();
        request->redirect("/saved");
        delay(3000);
        ESP.restart();
    });
}
