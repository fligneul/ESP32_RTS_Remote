#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>

/**
 * Singleton class
 * Handle the Wifi
 */
class WifiHandler_ {
   private:
    WifiHandler_() = default;  // Make constructor private

    Preferences preferences;
    bool isConfigured = false;

   public:
    static WifiHandler_ &getInstance();  // Accessor for singleton instance

    WifiHandler_(const WifiHandler_ &) = delete;  // no copying
    WifiHandler_ &operator=(const WifiHandler_ &) = delete;

   public:
    /**
     * Init the Wifi handler
     */
    void begin();

    /**
     * Return true is the Wifi is configured
     */
    bool configured();
    bool connected();

    void connect();
    void loop();
    void deleteConfig();
    void startAP();
};

extern WifiHandler_ &WifiHandler;