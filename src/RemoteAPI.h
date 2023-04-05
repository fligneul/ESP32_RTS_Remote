#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <map>

#include "Remote.h"

class RemoteAPI {
   private:
    AsyncWebServer *webServer;
    std::map<uint32_t, Remote> readConfig();
    std::map<uint32_t, Remote> readConfigWithRollingCode();
    void addRemote(Remote remote);
    bool removeRemote(uint32_t remoteId);
    void saveConfig(std::map<uint32_t, Remote> remotesMap);

   public:
    RemoteAPI(AsyncWebServer *webServer);
    void begin();
};
