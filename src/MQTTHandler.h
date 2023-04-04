#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <string.h>

/**
 * Handle the MQTT handler
 */
class MQTTHandler {
   private:
    bool isConfigured = false;
    Preferences preferences;

    void connect();

   public:
    void begin();
    void loop();
};
