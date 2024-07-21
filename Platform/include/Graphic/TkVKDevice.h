#ifndef TK_ENGINE_TK_VK_DEVICE_H
#define TK_ENGINE_TK_VK_DEVICE_H

#include "TkVKCommon.h"

namespace tk {
    class TkVKGraphicContext;

    class TkVKQueue;

    struct TkVKSettings {

    };

    class TkVKDevice {
    public:
        TkVKDevice(TkVKGraphicContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount, const TkVKSettings &settings = {});

        ~TkVKDevice();

    private:
        VkDevice mDevice{};
        std::vector<std::shared_ptr<TkVKQueue>> mGraphicQueues;
        std::vector<std::shared_ptr<TkVKQueue>> mPresentQueues;
    };
}

#endif //TK_ENGINE_TK_VK_DEVICE_H
