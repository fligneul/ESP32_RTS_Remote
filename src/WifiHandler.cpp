#include "WifiHandler.h"

#include "WifiConfig.h"

#define RECONNECT_INTERVAL 30000

unsigned long previousReconnect = 0;

void WifiHandler_::begin() {
    preferences.begin(NVS_WIFI_NAMESPACE, true);
    isConfigured = preferences.isKey(NVS_WIFI_SSID_KEY) &&
                   !preferences.getString(NVS_WIFI_SSID_KEY).isEmpty();
    preferences.end();
}

bool WifiHandler_::configured() { return isConfigured; }

bool WifiHandler_::connected() {
    return isConfigured && WiFi.status() == WL_CONNECTED;
}

void WifiHandler_::connect() {
    Serial.println("Connect to Wifi ");
    WiFi.mode(WIFI_STA);

    Preferences preferences;
    preferences.begin(NVS_WIFI_NAMESPACE, true);
    WiFi.setHostname(
        preferences
            .getString(NVS_WIFI_HOSTNAME_KEY, NVS_WIFI_HOSTNAME_KEY_DEFAULT)
            .c_str());
    WiFi.begin(preferences.getString(NVS_WIFI_SSID_KEY).c_str(),
               preferences.getString(NVS_WIFI_PASSWORD_KEY).c_str());
    WiFi.setSleep(false);
    preferences.end();

    if (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    delay(10);
    Serial.println(WiFi.localIP());
}

void WifiHandler_::loop() {
    if (isConfigured && WiFi.status() != WL_CONNECTED &&
        (millis() - previousReconnect >= RECONNECT_INTERVAL)) {
        WiFi.disconnect();
        WiFi.reconnect();
        previousReconnect = millis();
    }
}

void WifiHandler_::deleteConfig() {
    Preferences preferences;
    preferences.begin(NVS_WIFI_NAMESPACE, false);
    preferences.remove(NVS_WIFI_SSID_KEY);
    preferences.remove(NVS_WIFI_PASSWORD_KEY);
    preferences.remove(NVS_WIFI_HOSTNAME_KEY);
    preferences.end();
}

void WifiHandler_::startAP() {
    Serial.println("Starting AP");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(NVS_WIFI_HOSTNAME_KEY_DEFAULT, NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

// Singleton methods

WifiHandler_ &WifiHandler_::getInstance() {
    static WifiHandler_ instance;
    return instance;
}

WifiHandler_ &WifiHandler = WifiHandler.getInstance();