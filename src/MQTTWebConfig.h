#pragma once

#include <ESPAsyncWebServer.h>

class MQTTWebConfig {
   private:
    AsyncWebServer *webServer;

   public:
    MQTTWebConfig(AsyncWebServer *webServer);
    void begin();
};
