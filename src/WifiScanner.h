#pragma once

#include <Arduino.h>
#include <WiFi.h>

/**
 * Singleton class
 * Handle the Wifi
 */
class WifiScanner_ {
   private:
    WifiScanner_() = default;  // Make constructor private

   public:
    static WifiScanner_ &getInstance();  // Accessor for singleton instance

    WifiScanner_(const WifiScanner_ &) = delete;  // no copying
    WifiScanner_ &operator=(const WifiScanner_ &) = delete;

   public:
    /**
     * Scan available networks
     */
    void scan();
};

extern WifiScanner_ &WifiScanner;