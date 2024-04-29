#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <EasyButton.h>
#include <SPIFFS.h>
#include <WiFi.h>

#include "CommandHandler.h"
#include "Constants.h"
#include "MQTTHandler.h"
#include "MQTTWebConfig.h"
#include "OTAWebConfig.h"
#include "RemoteAPI.h"
#include "RemoteHandler.h"
#include "RemoteWebConfig.h"
#include "WifiHandler.h"
#include "WifiWebConfig.h"
#include "WifiScanner.h"

AsyncWebServer server(80);
WifiWebConfig wifiWebConfig(&server);
OTAWebConfig otaWebConfig(&server);
MQTTWebConfig mqttWebConfig(&server);
RemoteWebConfig remoteWebConfig(&server);
RemoteAPI remoteAPI(&server);

EasyButton progButton(GPIO_PROG_PIN);

void onLongPress() {
    digitalWrite(GPIO_LED_PIN, HIGH);
    delay(200);
    digitalWrite(GPIO_LED_PIN, LOW);
    delay(200);
    digitalWrite(GPIO_LED_PIN, HIGH);
    delay(200);
    digitalWrite(GPIO_LED_PIN, LOW);
    delay(200);
    WifiHandler.deleteConfig();
    ESP.restart();
}

void setup() {
    Serial.begin(115200);
    WifiHandler.begin();

    pinMode(GPIO_LED_PIN, OUTPUT);
    digitalWrite(GPIO_LED_PIN, LOW);

    pinMode(GPIO_PROG_PIN, INPUT_PULLUP);
    progButton.begin();
    progButton.onPressedFor(5000, onLongPress);

    if (!SPIFFS.begin(false)) {
        Serial.println("SPIFFS Mount Failed");
        delay(10000);
        ESP.restart();
    }

    if (!WifiHandler.configured()) {
        WifiHandler.startAP();
        wifiWebConfig.begin(true);
    } else {
        Serial.println("WiFi configured");
        WifiHandler.connect();

        // start API
        remoteAPI.begin();

        // Start web config ui
        wifiWebConfig.begin();
        otaWebConfig.begin();
        mqttWebConfig.begin();
        remoteWebConfig.begin();

        // Load remotes config
        RemoteHandler.begin();

        // Start MQTT handler
        MQTTHandler.begin();
    }

    // Start wifi scanner
    WifiScanner.scan();

    // Static files
    server.serveStatic("/", SPIFFS, "/");

    // ESP Restart endpoint
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
        ESP.restart();
    });

    // Saved page
    server.on("/saved", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/saved.html", "text/html"); });

    // Wifi config page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/index.html", "text/html"); });

    // Start server
    server.begin();
}

void loop() {
    WifiHandler.loop();
    MQTTHandler.loop();

    CommandHandler.handle();

    progButton.read();
    delay(5);
}
