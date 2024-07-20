#include "TkGraphicContext.h"
#include "Graphic/TkVKGraphicContext.h"

namespace tk {
    std::unique_ptr<TkGraphicContext> TkGraphicContext::Create(TkWindow *window) {
#ifdef TK_ENGINE_GRAPHIC_API_VULKAN
        return std::make_unique<TkVKGraphicContext>(window);
#else

#endif
        return nullptr;
    }
}