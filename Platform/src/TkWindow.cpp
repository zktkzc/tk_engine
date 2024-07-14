#include "TkWindow.h"
#include "Window/TkGLFWWindow.h"

namespace tk {
    std::unique_ptr<TkWindow> TkWindow::Create(uint32_t width, uint32_t height, const char *title) {
#ifdef TK_ENGINE_PLATFORM_WIN32
    return std::make_unique<TkGLFWWindow>(width, height, title);
#elif TK_ENGINE_PLATFORM_MACOS
        return std::make_unique<TkGLFWWindow>(width, height, title);
#elif TK_ENGINE_PLATFORM_LINUX
return std::make_unique<TkGLFWWindow>(width, height, title);
#endif
        return nullptr;
    }
}