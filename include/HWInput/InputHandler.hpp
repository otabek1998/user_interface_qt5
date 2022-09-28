#pragma once

#include "Common.hpp"

#include <memory>
#include <set>

#include <dlt.h>

DLT_IMPORT_CONTEXT(HWInputDltContext);

namespace HWInput
{
    class ICallback;

    /**
     * @brief This class handles the hardware button input provided by Power Processor
     *          IPC module via DBus signals and invokes the provided callbacks for
     *          button press/release events.
     */
    class InputHandler
    {
    public:
        InputHandler();

        /**
         * @brief Initialize the input handler
         * @param pCallback Pointer to the callback interface
         * @return true if initialization is successful, false otherwise
         */
        bool init(ICallback* pCallback);

        /**
         * @brief Defines a hot key
         * @param keys The keys that must be pressed in combination in order to generate the onHotKey event
         * @param id The identifier of the hot key
         * @return return true if the function succeeds
         */
        bool registerHotKey(const std::set<Button>& keys, HotKeyId& id);

        /**
         * @brief Frees a hot key previously registered by the calling registerHotKey
         * @param handle The identifier of the hot key to be freed
         */
        void unregisterHotKey(HotKeyId id);

        /**
         * @brief pause touch events and D-Bus signal processing
         */
        void pauseProcessing();

        /**
         * @brief resume touch events and D-Bus signal processing
         */
        void resumeProcessing(); 

        ~InputHandler();

    private:
        class Impl;
        std::unique_ptr<Impl> mImpl;
    };
}
