#include "CommandQueue.h"

#define QUEUE_SIZE_ITEMS 20

// The Command queue
ArduinoQueue<RemoteCommand> commandQueue(QUEUE_SIZE_ITEMS);

bool CommandQueue_::commandWaiting() { return !commandQueue.isEmpty(); }

void CommandQueue_::registerCommand(const RemoteCommand command) {
    commandQueue.enqueue(command);
}

RemoteCommand CommandQueue_::getCommand() { return commandQueue.dequeue(); }

// Singleton methods

CommandQueue_ &CommandQueue_::getInstance() {
    static CommandQueue_ instance;
    return instance;
}

CommandQueue_ &CommandQueue = CommandQueue.getInstance();