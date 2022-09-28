#pragma once
#include "Common.hpp"

namespace HWInput
{
    /**
     * @brief Hardware input callback interface
     */
    class ICallback
    {
    public:
        virtual void onButtonDown(Button controlId, ButtonType buttonType) = 0;
        virtual void onButtonUp(Button controlId, ButtonType buttonType) = 0;
        virtual void onHotKey(HotKeyId id) = 0;

        virtual ~ICallback() = default;
    };
}
