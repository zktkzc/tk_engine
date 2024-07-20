#ifndef TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H
#define TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H

#include "TkGraphicContext.h"
#include "Graphic/TkVKCommon.h"

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

    class TkVKGraphicContext : public TkGraphicContext {
    public:
        explicit TkVKGraphicContext(TkWindow *window);

        ~TkVKGraphicContext() override;

    private:
        void CreateInstance();

        void CreateSurface(TkWindow *window);

    private:
        VkInstance mInstance = nullptr;
        VkSurfaceKHR mSurface = nullptr;
        bool bShouldValidate = true;
    };
}

#endif //TK_ENGINE_TK_VK_GRAPHIC_CONTEXT_H