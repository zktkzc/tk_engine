#include "Graphic/TkVKDevice.h"
#include "Graphic/TkVKGraphicContext.h"
#include "Graphic/TkVKQueue.h"

namespace tk {
    const DeviceFuture requestedExtensions[] = {
            {VK_KHR_SWAPCHAIN_EXTENSION_NAME, true},
#ifdef TK_ENGINE_PLATFORM_WIN32
#endif
#ifdef TK_ENGINE_PLATFORM_MACOS
            {"VK_KHR_portability_subset", true},
#endif
#ifdef TK_ENGINE_PLATFORM_LINUX
#endif
    };

    TkVKDevice::TkVKDevice(TkVKGraphicContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount, const TkVKSettings &settings) {
        if (!context) {
            LOG_E("Must create a vulkan context before creating a device.");
            return;
        }

        QueueFamilyInfo graphicQueueFamilyInfo = context->GetGraphicQueueFamilyInfo();
        QueueFamilyInfo presentQueueFamilyInfo = context->GetPresentQueueFamilyInfo();

        if (graphicQueueCount > graphicQueueFamilyInfo.queueCount) {
            LOG_E("Graphic queue family has {0} queues, but request {1} queues.", graphicQueueFamilyInfo.queueCount, graphicQueueCount);
            return;
        }

        if (presentQueueCount > presentQueueFamilyInfo.queueCount) {
            LOG_E("Present queue family has {0} queues, but request {1} queues.", presentQueueFamilyInfo.queueCount, presentQueueCount);
            return;
        }

        std::vector<float> graphicQueuePriorities(graphicQueueCount, 0.0f);
        std::vector<float> presentQueuePriorities(presentQueueCount, 1.0f);
        bool bSameQueueFamilyIndex = context->IsSameQueueFamily();
        uint32_t sameQueueCount = graphicQueueCount;
        if (bSameQueueFamilyIndex) {
            sameQueueCount += presentQueueCount;
            if (sameQueueCount > graphicQueueFamilyInfo.queueCount) {
                sameQueueCount = graphicQueueFamilyInfo.queueCount;
            }
            graphicQueuePriorities.insert(graphicQueuePriorities.end(), presentQueuePriorities.begin(), presentQueuePriorities.end());
        }
        VkDeviceQueueCreateInfo queueCreateInfos[2] = {
                {
                        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                        .pNext = nullptr,
                        .flags = 0,
                        .queueFamilyIndex = static_cast<uint32_t>(graphicQueueFamilyInfo.queueFamilyIndex),
                        .queueCount = sameQueueCount,
                        .pQueuePriorities = graphicQueuePriorities.data()
                }
        };

        if (!bSameQueueFamilyIndex) {
            queueCreateInfos[1] = {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .queueFamilyIndex = static_cast<uint32_t>(presentQueueFamilyInfo.queueFamilyIndex),
                    .queueCount = presentQueueCount,
                    .pQueuePriorities = presentQueuePriorities.data()
            };
        }

        uint32_t availableExtensionCount = 0;
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(), "", &availableExtensionCount, nullptr));
        VkExtensionProperties availableExtensions[availableExtensionCount];
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(), "", &availableExtensionCount, availableExtensions));

        uint32_t enableExtensionCount = 0;
        const char *enableExtensions[32];
        if (!checkDeviceFutures("Device Extensions", true, availableExtensionCount, availableExtensions, ARRAY_SIZE(requestedExtensions), requestedExtensions, &enableExtensionCount, enableExtensions)) {
            return;
        }

        VkDeviceCreateInfo deviceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueCreateInfoCount = static_cast<uint32_t>(bSameQueueFamilyIndex ? 1 : 2),
                .pQueueCreateInfos = queueCreateInfos,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = enableExtensionCount,
                .ppEnabledExtensionNames = enableExtensionCount > 0 ? enableExtensions : nullptr,
                .pEnabledFeatures = nullptr,
        };
        CALL_VK(vkCreateDevice(context->GetPhysicalDevice(), &deviceCreateInfo, nullptr, &mDevice));
        LOG_T("Create Device Success : {0}", (void*)mDevice);

        for (int i = 0; i < graphicQueueCount; i++) {
            VkQueue queue;
            vkGetDeviceQueue(mDevice, graphicQueueFamilyInfo.queueFamilyIndex, i, &queue);
            mGraphicQueues.push_back(std::make_shared<TkVKQueue>(graphicQueueFamilyInfo.queueFamilyIndex, i, queue, false));
        }
        for (int i = 0; i < presentQueueCount; i++) {
            VkQueue queue;
            vkGetDeviceQueue(mDevice, presentQueueFamilyInfo.queueFamilyIndex, i, &queue);
            mPresentQueues.push_back(std::make_shared<TkVKQueue>(presentQueueFamilyInfo.queueFamilyIndex, i, queue, true));
        }
    }

    TkVKDevice::~TkVKDevice() {
        vkDeviceWaitIdle(mDevice);
        vkDestroyDevice(mDevice, nullptr);
    }
}