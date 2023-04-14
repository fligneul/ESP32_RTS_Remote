#pragma once

/**
 * Singleton class
 * Handle a queued processed remote command
 */
class CommandHandler_ {
   private:
    CommandHandler_() = default;  // Make constructor private

   public:
    static CommandHandler_ &getInstance();  // Accessor for singleton instance

    CommandHandler_(const CommandHandler_ &) = delete;  // no copying
    CommandHandler_ &operator=(const CommandHandler_ &) = delete;

    void handle();
};

extern CommandHandler_ &CommandHandler;
