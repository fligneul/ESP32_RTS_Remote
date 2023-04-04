#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>

class WifiWebConfig {
   private:
    AsyncWebServer *webServer;

   public:
    WifiWebConfig(AsyncWebServer *webServer);
    void begin(const bool accessPoint);
};
