#ifndef TK_ENGINE_TK_GRAPHIC_CONTEXT_H
#define TK_ENGINE_TK_GRAPHIC_CONTEXT_H

#include "TkEngine.h"

namespace tk {
    class TkWindow;

    class TkGraphicContext {
    public:
        TkGraphicContext(const TkGraphicContext &) = delete;

        TkGraphicContext &operator=(const TkGraphicContext &) = delete;

        virtual ~TkGraphicContext() = default;

        static std::unique_ptr<TkGraphicContext> Create(TkWindow *window);

    protected:
        TkGraphicContext() = default;
    };
}

#endif //TK_ENGINE_TK_GRAPHIC_CONTEXT_H
