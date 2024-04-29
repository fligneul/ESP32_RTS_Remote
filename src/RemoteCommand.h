#pragma once

#include <Arduino.h>

struct RemoteCommand {
    uint32_t remoteID;
    String rawCommand;
};