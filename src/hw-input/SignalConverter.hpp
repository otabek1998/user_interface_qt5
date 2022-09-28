#pragma once
#include <memory>
#include "HWInput/ICallback.hpp"

namespace HWInput
{
    //--------------------------------------------------------------------------
    class SignalConverter
    {
    public:
        /**
         * @brief Constructor
         * @param pCallback HW button callback pointer
         */
        explicit SignalConverter(ICallback* pCallback);

        /**
         * @brief Connect to D-Bus and subscribe to required HW button signals
         * @return true if successful, false otherwise
         */
        bool connect();

        /**
         * @brief Read and process messages from D-Bus connection.
         */
        void process();

        /**
         * @brief Wake up D-Bus loop
         */
        void wakeUp();

        /**
         * @brief pause D-Bus signal processing
         */
        void pauseProcessing();

        /**
         * @brief resume D-Bus signal processing
         */
        void resumeProcessing();

        ~SignalConverter();
    private:
        struct Impl;
        std::unique_ptr<Impl> mImpl;
    };
}