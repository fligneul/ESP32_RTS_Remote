#pragma once

#include <Arduino.h>

struct Remote {
    uint32_t id;
    String name;
    uint16_t rollingCode;
};
