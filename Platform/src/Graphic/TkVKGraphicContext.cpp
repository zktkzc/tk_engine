#include "Graphic/TkVKGraphicContext.h"
#include "Window/TkGLFWWindow.h"

namespace tk {
    const DeviceFuture requestedLayers[] = {
            {"VK_LAYER_KHRONOS_validation", true},
    };

    const DeviceFuture requestedExtensions[] = {
            {VK_KHR_SURFACE_EXTENSION_NAME, true},
            {VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false},
#ifdef TK_ENGINE_PLATFORM_WIN32
            {VK_KHR_WIN32_SURFACE_EXTENSION_NAME, true},
#endif
#ifdef TK_ENGINE_PLATFORM_MACOS
            {VK_MVK_MACOS_SURFACE_EXTENSION_NAME, true},
#endif
#ifdef TK_ENGINE_PLATFORM_LINUX
            {VK_KHR_XCB_SURFACE_EXTENSION_NAME, true},
#endif
    };

    TkVKGraphicContext::TkVKGraphicContext(TkWindow *window) {
        CreateInstance();
        CreateSurface(window);
        SelectPhyDevice();
    }

    TkVKGraphicContext::~TkVKGraphicContext() {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        vkDestroyInstance(mInstance, nullptr);
    }

    static VkBool32 VkDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData) {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            LOG_E("{0} : {1}", pLayerPrefix, pMessage);
        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            LOG_W("{0} : {1}", pLayerPrefix, pMessage);
        }
        return VK_TRUE;
    }

    void TkVKGraphicContext::CreateInstance() {
        // 构建支持的层layers
        uint32_t availableLayerCount = 0;
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr));
        VkLayerProperties availableLayers[availableLayerCount];
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

        uint32_t enableLayerCount = 0;
        const char *enableLayers[32];
        if (bShouldValidate) {
            if (!checkDeviceFutures("Instance Layers", false, availableLayerCount, availableLayers, ARRAY_SIZE(requestedLayers), requestedLayers, &enableLayerCount, enableLayers)) {
                return;
            }
        }

        // 构建扩展extensions
        uint32_t availableExtensionCount = 0;
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, nullptr));
        VkExtensionProperties availableExtensions[availableExtensionCount];
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, availableExtensions));

        uint32_t GLFWRequestedExtensionCount = 0;
        const char **GLFWRequestedExtensions = glfwGetRequiredInstanceExtensions(&GLFWRequestedExtensionCount);
        std::unordered_set<const char *> allRequestedExtensionsSet;
        std::vector<DeviceFuture> allRequestedExtensions;
        for (const auto &item: requestedExtensions) {
            if (allRequestedExtensionsSet.find(item.name) == allRequestedExtensionsSet.end()) {
                allRequestedExtensionsSet.insert(item.name);
                allRequestedExtensions.push_back(item);
            }
        }
        for (int i = 0; i < GLFWRequestedExtensionCount; i++) {
            auto extensionName = GLFWRequestedExtensions[i];
            if (allRequestedExtensionsSet.find(extensionName) == allRequestedExtensionsSet.end()) {
                allRequestedExtensionsSet.insert(extensionName);
                allRequestedExtensions.push_back({extensionName, true});
            }
        }

        uint32_t enableExtensionCount = 0;
        const char *enableExtensions[32];
        if (!checkDeviceFutures("Instance Extensions", true, availableExtensionCount, availableExtensions, allRequestedExtensions.size(), allRequestedExtensions.data(), &enableExtensionCount, enableExtensions)) {
            return;
        }

        // 创建vulkan实例
        VkApplicationInfo applicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = "TkEngine",
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                .pEngineName = "TkEngine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_3,
        };

        VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfoExt{
                .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                .pNext = nullptr,
                .flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
                .pfnCallback = VkDebugReportCallback
        };

        VkInstanceCreateInfo instanceInfo{
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = bShouldValidate ? &debugReportCallbackCreateInfoExt : nullptr,
                .flags = 0,
                .pApplicationInfo = &applicationInfo,
                .enabledLayerCount = enableLayerCount,
                .ppEnabledLayerNames = enableLayerCount > 0 ? enableLayers : nullptr,
                .enabledExtensionCount = enableExtensionCount,
                .ppEnabledExtensionNames = enableExtensionCount > 0 ? enableExtensions : nullptr,
        };
        CALL_VK(vkCreateInstance(&instanceInfo, nullptr, &mInstance));
        LOG_T("{0} : instance : {1}", __FUNCTION__, (void *) mInstance);
    }

    void TkVKGraphicContext::CreateSurface(TkWindow *window) {
        if (!window) {
            LOG_E("window is not exists.");
            return;
        }
        auto GLFWWindow = dynamic_cast<TkGLFWWindow *>(window);
        if (!GLFWWindow) {
            // FIXME
            LOG_E("window is not a GLFWWindow.");
            return;
        }
        CALL_VK(glfwCreateWindowSurface(mInstance, GLFWWindow->GetWindowHandler(), nullptr, &mSurface));
        LOG_T("{0} : surface : {1}", __FUNCTION__, (void *) mSurface);
    }

    void TkVKGraphicContext::SelectPhyDevice() {
        // 查询当前环境下的所有物理设备
        uint32_t physicalDeviceCount = 0;
        CALL_VK(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr));
        VkPhysicalDevice physicalDevices[physicalDeviceCount];
        CALL_VK(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices));

        uint32_t maxScore = 0;
        int32_t maxScorePhyDeviceIndex = -1;
        for (int i = 0; i < physicalDeviceCount; ++i) {
            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceProperties);
            PrintPhyDeviceInfo(physicalDeviceProperties);

            uint32_t deviceScore = GetPhyDeviceScore(physicalDeviceProperties);
            uint32_t formatCount;
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], mSurface, &formatCount, nullptr));
            VkSurfaceFormatKHR formats[formatCount];
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], mSurface, &formatCount, formats));
            for (uint32_t j = 0; j < formatCount; ++j) {
                if (formats[j].format == VK_FORMAT_B8G8R8A8_UNORM && formats[j].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    deviceScore += 10;
                    break;
                }
            }

            // 查询物理设备对应的队列族
            uint32_t queueFamilyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, nullptr);
            VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilyProperties);

            LOG_D("-------------------- [ Physical device {0} ] --------------------", i);
            LOG_D("Physical device {0} score: {1}", i, deviceScore);
            LOG_D("Physical device {0} queue family count: {1}", i, queueFamilyCount);
            LOG_D("---------------------------------------------------------------");

            if (deviceScore < maxScore) {
                continue;
            }

            for (uint32_t j = 0; j < queueFamilyCount; ++j) {
                if (queueFamilyProperties[j].queueCount == 0) {
                    continue;
                }

                // graphic family 图形渲染
                if (queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    mGraphicQueueFamily.queueFamilyIndex = (int32_t) j;
                    mGraphicQueueFamily.queueCount = queueFamilyProperties[j].queueCount;
                }

                if (
                        mGraphicQueueFamily.queueFamilyIndex >= 0
                        && mPresentQueueFamily.queueFamilyIndex >= 0
                        && mPresentQueueFamily.queueFamilyIndex != mGraphicQueueFamily.queueFamilyIndex
                        ) {
                    break;
                }

                // present family 显示
                VkBool32 bSupportSurface;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, mSurface, &bSupportSurface);
                if (bSupportSurface) {
                    mPresentQueueFamily.queueFamilyIndex = (int32_t) j;
                    mPresentQueueFamily.queueCount = queueFamilyProperties[j].queueCount;
                }
            }

            if (mGraphicQueueFamily.queueFamilyIndex >= 0 && mPresentQueueFamily.queueFamilyIndex >= 0) {
                maxScorePhyDeviceIndex = i;
                maxScore = deviceScore;
            }
        }

        if (maxScorePhyDeviceIndex < 0) {
            LOG_W("Maybe can not find a suitable physical device, will use the first device in the list.");
            maxScorePhyDeviceIndex = 0;
        }

        mPhyDevice = physicalDevices[maxScorePhyDeviceIndex];
        vkGetPhysicalDeviceMemoryProperties(mPhyDevice, &mPhyDeviceMemoryProperties);
        LOG_T("{0} : physical device: {1}, score: {2}, graphic queue: {3} : {4}, present queue: {5} : {6}", __FUNCTION__, maxScorePhyDeviceIndex, maxScore, mGraphicQueueFamily.queueFamilyIndex, mGraphicQueueFamily.queueCount, mPresentQueueFamily.queueFamilyIndex, mPresentQueueFamily.queueCount);
    }

    void TkVKGraphicContext::PrintPhyDeviceInfo(const VkPhysicalDeviceProperties &props) {
        const char *deviceType = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated GPU" : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete GPU" : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual GPU" : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "CPU" : "Unknown";
        uint32_t driverVersionMajor = VK_VERSION_MAJOR(props.driverVersion);
        uint32_t driverVersionMinor = VK_VERSION_MINOR(props.driverVersion);
        uint32_t driverVersionPatch = VK_VERSION_PATCH(props.driverVersion);

        uint32_t apiVersionMajor = VK_VERSION_MAJOR(props.apiVersion);
        uint32_t apiVersionMinor = VK_VERSION_MINOR(props.apiVersion);
        uint32_t apiVersionPatch = VK_VERSION_PATCH(props.apiVersion);

        LOG_D("----------------[ Physical Device Info ]-----------------");
        LOG_D("deviceName       : {0}", props.deviceName);
        LOG_D("deviceType       : {0}", deviceType);
        LOG_D("vendorID         : {0}", props.vendorID);
        LOG_D("deviceID         : {0}", props.deviceID);
        LOG_D("driverVersion    : {0}.{1}.{2}", driverVersionMajor, driverVersionMinor, driverVersionPatch);
        LOG_D("apiVersion       : {0}.{1}.{2}", apiVersionMajor, apiVersionMinor, apiVersionPatch);
        LOG_D("---------------------------------------------------------");
    }

    uint32_t TkVKGraphicContext::GetPhyDeviceScore(const VkPhysicalDeviceProperties &props) {
        VkPhysicalDeviceType deviceType = props.deviceType;
        uint32_t score = 0;
        switch (deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += 40;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 30;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += 20;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += 10;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
                break;
        }
        return score;
    }
}