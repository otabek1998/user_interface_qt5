#pragma once
#include <cstdint>

namespace HWInput
{
    using HotKeyId = uint32_t;

    enum class Button: uint8_t
    {
        BUTTON_HOME         = 1,
        BUTTON_MUTE         = 2,
        BUTTON_POWER        = 3,
        BUTTON_VOLUP        = 4,
        BUTTON_VOLDN        = 5,
        //@ Softkey voice dial
        BUTTON_VOICEDIAL    = 6,
        BUTTON_SETTING      = 7,
        BUTTON_SEEKUP       = 8,
        BUTTON_SEEKDN       = 9,
        BUTTON_TELON        = 10,
        BUTTON_TELOFF       = 11,
        BUTTON_SOURCE       = 12,
        BUTTON_FLASH        = 13,
        BUTTON_MUSIC        = 14,
        BUTTON_PHONE        = 15,
        //@ Steering wheel voice dial
        BUTTON_VOICEDIALSW  = 16
    };

    enum class ButtonType: uint8_t
    {
        HARDKEY     =   1,
        SOFTKEY     =   2,
        STEERING_SW =   3
    };
}
