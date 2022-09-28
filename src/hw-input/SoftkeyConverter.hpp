#pragma once
#include <atomic>
#include <map>
#include <utility>
#include <stdint.h>

#include "HWInput/Common.hpp"

struct libinput;
struct libinput_event;

namespace HWInput
{
    class ICallback;

    class SoftkeyConverter
    {
    public:
        /**
         * @brief Constructor
         * @param pCallback HW button callback pointer
         */
        explicit SoftkeyConverter(ICallback* pCallback);
        ~SoftkeyConverter();

        SoftkeyConverter(const SoftkeyConverter&) = delete;
        SoftkeyConverter& operator==(const SoftkeyConverter&) = delete;

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
         * @brief pause touch events processing
         */
        void pauseProcessing()
        {
            m_pauseProcessing = true;
        }

        /**
         * @brief resume touch events processing
         */
        void resumeProcessing()
        {
            m_pauseProcessing = false;
        }

    private:
        static const std::map<std::pair<long, long>, Button> CoordinatesToButtonMap;

        ICallback* m_pCallback{nullptr};
        libinput* m_pInputCtx{nullptr};
        std::atomic_bool m_running{false};
        std::atomic_bool m_pauseProcessing{false};
        std::map<int32_t, Button> m_slotToButton;

        void processEvents();
        void processTouchDownEvent(libinput_event* ev);
        void processTouchUpEvent(libinput_event* ev);
        std::pair<bool, Button> cooordinatesToButton(double x, double y) const;
    };
}
