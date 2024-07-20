#include "Graphic/TkVKGraphicContext.h"
#include "Window/TkGLFWWindow.h"

namespace tk {
    TkVKGraphicContext::TkVKGraphicContext(TkWindow *window) {
        CreateInstance();
        CreateSurface(window);
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
}