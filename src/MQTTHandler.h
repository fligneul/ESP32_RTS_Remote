#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <string.h>

/**
 * Singleton class
 * Handle the MQTT messages
 */
class MQTTHandler_ {
   private:
    MQTTHandler_() = default;  // Make constructor private

    bool isConfigured = false;
    Preferences preferences;

    void connect();

   public:
    static MQTTHandler_ &getInstance();  // Accessor for singleton instance

    MQTTHandler_(const MQTTHandler_ &) = delete;  // no copying
    MQTTHandler_ &operator=(const MQTTHandler_ &) = delete;

    void begin();
    void loop();
};

extern MQTTHandler_ &MQTTHandler;
