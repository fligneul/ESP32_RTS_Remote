#include "WifiScanner.h"

void WifiScanner_::scan() {
    WiFi.scanNetworks(true);
}

// Singleton methods
WifiScanner_ &WifiScanner_::getInstance() {
    static WifiScanner_ instance;
    return instance;
}

WifiScanner_ &WifiScanner = WifiScanner.getInstance();