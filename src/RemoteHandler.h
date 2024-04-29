#pragma once

#include <SomfyRemote.h>

#include <map>

/**
 * Singleton class
 * Handle the remotes
 */
class RemoteHandler_ {
   private:
    RemoteHandler_() = default;  // Make constructor private

   public:
    static RemoteHandler_ &getInstance();  // Accessor for singleton instance

    RemoteHandler_(const RemoteHandler_ &) = delete;  // no copying
    RemoteHandler_ &operator=(const RemoteHandler_ &) = delete;

    void begin();
    std::map<uint32_t, SomfyRemote *> *getRemoteMap();
};

extern RemoteHandler_ &RemoteHandler;
