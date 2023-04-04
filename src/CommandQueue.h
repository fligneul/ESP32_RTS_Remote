#pragma once

#include <ArduinoQueue.h>

#include "RemoteCommand.h"

/**
 * Singleton class
 * Handle the Command Queue
 */
class CommandQueue_ {
   private:
    CommandQueue_() = default;  // Make constructor private

   public:
    static CommandQueue_ &getInstance();  // Accessor for singleton instance

    CommandQueue_(const CommandQueue_ &) = delete;  // no copying
    CommandQueue_ &operator=(const CommandQueue_ &) = delete;

   public:
    /**
     * Return true if a command is present in the queue
     */
    bool commandWaiting();
    /**
     * Register a new command in the queue
     */
    void registerCommand(const RemoteCommand command);

    /**
     * Dequeue and return a Command
     */
    RemoteCommand getCommand();
};

extern CommandQueue_ &CommandQueue;