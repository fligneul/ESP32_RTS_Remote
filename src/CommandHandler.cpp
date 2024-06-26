#include "CommandHandler.h"

#include <Preferences.h>
#include <SomfyRemote.h>

#include <map>

#include "CommandQueue.h"
#include "Constants.h"
#include "RemoteCommand.h"
#include "RemoteHandler.h"

#define COMMAND_INTERVAL 50

unsigned long lastSend = 0;

// Handle waiting command
// Should be called at each loop
void CommandHandler_::handle() {
    if (CommandQueue.commandWaiting() && ((millis() - lastSend) > COMMAND_INTERVAL)) {
        RemoteCommand remoteCommand = CommandQueue.getCommand();

        Serial.println(remoteCommand.remoteID);

        auto search = RemoteHandler.getRemoteMap()->find(remoteCommand.remoteID);
        if (search != RemoteHandler.getRemoteMap()->end()) {
            const Command command = getSomfyCommand(remoteCommand.rawCommand);
            search->second->sendCommand(command, 3);
        } else {
            Serial.println("Remote not found");
        }
        lastSend = millis();
    }
}

// Singleton methods

CommandHandler_ &CommandHandler_::getInstance() {
    static CommandHandler_ instance;
    return instance;
}

CommandHandler_ &CommandHandler = CommandHandler.getInstance();