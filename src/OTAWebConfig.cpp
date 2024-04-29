#include "OTAWebConfig.h"

#include <SPIFFS.h>

#include "OTAHandler.h"

OTAHandler otaHandler;

OTAWebConfig::OTAWebConfig(AsyncWebServer *webServer) : webServer(webServer) {}

void OTAWebConfig::begin() {
    webServer->on("/ota", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/ota.html", "text/html");
    });

    webServer->on(
        "/ota", HTTP_POST, [](AsyncWebServerRequest *request) {},
        [](AsyncWebServerRequest *request, const String &filename, size_t index,
           uint8_t *data, size_t len, bool final) {
            otaHandler.update(request, filename, index, data, len, final);
        });
}
