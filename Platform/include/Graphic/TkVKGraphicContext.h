#ifndef TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H
#define TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H

#include "TkGraphicContext.h"
#include "Graphic/TkVKCommon.h"

namespace tk {
    struct QueueFamilyInfo {
        int32_t queueFamilyIndex = -1;
        uint32_t queueCount = 0;
    };

    class TkVKGraphicContext : public TkGraphicContext {
    public:
        explicit TkVKGraphicContext(TkWindow *window);

        ~TkVKGraphicContext() override;

        [[nodiscard]] VkInstance GetInstance() const { return mInstance; }

        [[nodiscard]] VkSurfaceKHR GetSurface() const { return mSurface; }

        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return mPhyDevice; }

        [[nodiscard]] const QueueFamilyInfo &GetGraphicQueueFamilyInfo() const { return mGraphicQueueFamily; }

        [[nodiscard]] const QueueFamilyInfo &GetPresentQueueFamilyInfo() const { return mPresentQueueFamily; }

        [[nodiscard]] VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties() const { return mPhyDeviceMemoryProperties; }

        [[nodiscard]] bool IsSameQueueFamily() const { return mGraphicQueueFamily.queueFamilyIndex == mPresentQueueFamily.queueFamilyIndex; }

    private:
        void CreateInstance();

        void CreateSurface(TkWindow *window);

        void SelectPhyDevice();

        static void PrintPhyDeviceInfo(const VkPhysicalDeviceProperties &props);

        static uint32_t GetPhyDeviceScore(const VkPhysicalDeviceProperties &props);

    private:
        bool bShouldValidate = true;
        VkInstance mInstance = nullptr;
        VkSurfaceKHR mSurface = nullptr;

        VkPhysicalDevice mPhyDevice = nullptr;
        QueueFamilyInfo mGraphicQueueFamily{};
        QueueFamilyInfo mPresentQueueFamily{};
        VkPhysicalDeviceMemoryProperties mPhyDeviceMemoryProperties{};
    };
}

#endif //TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H