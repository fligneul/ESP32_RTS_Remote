#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <map>

class RemoteWebConfig {
   private:
    AsyncWebServer *webServer;

   public:
    RemoteWebConfig(AsyncWebServer *webServer);
    void begin();
};
