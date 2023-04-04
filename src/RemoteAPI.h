#pragma once

#include "Remote.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <map>

class RemoteAPI {
   private:
    AsyncWebServer *webServer;
    std::map<uint32_t, Remote> readConfig();

   public:
    RemoteAPI(AsyncWebServer *webServer);
    void begin();
};
