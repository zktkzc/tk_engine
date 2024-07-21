#include "Graphic/TkVKQueue.h"

namespace tk {
    TkVKQueue::TkVKQueue(uint32_t familyIndex, uint32_t queueIndex, VkQueue queue, bool isPresent): mFamilyIndex(familyIndex), mQueueIndex(queueIndex), mQueue(queue), bIsPresent(isPresent) {
        LOG_T("Create a new queue : {0} - {1} - {2}, present : {3}.", mFamilyIndex, mQueueIndex, (void*)mQueue, bIsPresent);
    }

    /**
     * 等待队列处理完成
     */
    void TkVKQueue::WaitIdle() const {
        if (mQueue) {
            CALL_VK(vkQueueWaitIdle(mQueue));
        }
    }
}