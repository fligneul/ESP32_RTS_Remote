#pragma once

#include <ESPAsyncWebServer.h>

class OTAHandler {
   private:
   public:
    void update(AsyncWebServerRequest *request, const String &filename,
                size_t index, uint8_t *data, size_t len, bool final);
};
