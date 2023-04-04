#pragma once

#include <ESPAsyncWebServer.h>

class OTAWebConfig {
   private:
    AsyncWebServer *webServer;

   public:
    OTAWebConfig(AsyncWebServer *webServer);
    void begin();
};
