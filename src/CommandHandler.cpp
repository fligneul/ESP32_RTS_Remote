#include "CommandHandler.h"

#include <Preferences.h>
#include <SomfyRemote.h>

#include <map>

#include "CommandQueue.h"
#include "Constants.h"
#include "RemoteCommand.h"
#include "RemoteHandler.h"

// Handle waiting command
// Should be called at each loop
void CommandHandler::handle() {
    if (CommandQueue.commandWaiting()) {
        RemoteCommand remoteCommand = CommandQueue.getCommand();

        Serial.println(remoteCommand.remoteID);

        auto search = RemoteHandler.getRemoteMap()->find(remoteCommand.remoteID);
        if (search != RemoteHandler.getRemoteMap()->end()) {
            const Command command = getSomfyCommand(remoteCommand.rawCommand);
            search->second->sendCommand(command, 0);
        } else {
            Serial.println("Remote not found");
        }
    }
}
