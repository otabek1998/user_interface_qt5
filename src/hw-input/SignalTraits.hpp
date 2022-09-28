#pragma once
#include <string>

#include "HWInput/Common.hpp"

namespace HWInput
{
    //--------------------------------------------------------------------------
    /**
     * @brief Button signal descriptor.
     * @description Provides signal name, reply signal name and Populus control
     * ID
     */
    struct SignalTraits
    {
        /**
         * Constructor.
         * @param button Button name from PP IPC spec (POWER, VOLUP, VOLDN, etc)
         * @param control Populus control ID
         */
        SignalTraits(const std::string& button, Button control);

        SignalTraits() = default;

        std::string signal;
        std::string reply;
        Button control;
    };
}