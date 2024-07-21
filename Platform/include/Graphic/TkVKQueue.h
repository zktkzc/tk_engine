#ifndef TK_ENGINE_TK_VK_QUEUE_H
#define TK_ENGINE_TK_VK_QUEUE_H

#include "TkVKCommon.h"

namespace tk {
    class TkVKQueue {
    public:
        TkVKQueue(uint32_t familyIndex, uint32_t queueIndex, VkQueue queue, bool isPresent);

        ~TkVKQueue() = default;

        void WaitIdle() const;

    private:
        uint32_t mFamilyIndex = 0;
        uint32_t mQueueIndex = 0;
        VkQueue mQueue{};
        bool bIsPresent = false;
    };
}

#endif //TK_ENGINE_TK_VK_QUEUE_H
