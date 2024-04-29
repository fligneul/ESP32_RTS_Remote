#include "OTAHandler.h"

#include <SPIFFS.h>
#include <Update.h>

void OTAHandler::update(AsyncWebServerRequest *request, const String &filename,
                        size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
        Serial.println("OTA Update");
        size_t content_len = request->contentLength();
        // if filename includes spiffs, update the spiffs partition
        int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
            Update.printError(Serial);
        }
    }

    if (Update.write(data, len) != len) {
        Update.printError(Serial);
    }

    if (final) {
        AsyncWebServerResponse *response = request->beginResponse(
            302, "text/plain", "Please wait while the device reboots");
        response->addHeader("Refresh", "20");
        response->addHeader("Location", "/");
        request->send(response);
        if (!Update.end(true)) {
            Update.printError(Serial);
        } else {
            Serial.println("OTA Update complete, restarting");
            Serial.flush();
            ESP.restart();
        }
    }
}
