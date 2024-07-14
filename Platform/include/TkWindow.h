#ifndef TK_ENGINE_TK_WINDOW_H
#define TK_ENGINE_TK_WINDOW_H

#include "TkEngine.h"

namespace tk {
    class TkWindow {
    public:
        TkWindow(const TkWindow &) = delete;

        TkWindow &operator=(const TkWindow &) = delete;

        virtual ~TkWindow() = default;

        TkWindow() = default;

        static std::unique_ptr<TkWindow> Create(uint32_t width, uint32_t height, const char *title);

        virtual bool ShouldClose() = 0;

        virtual void PollEvents() = 0;

        virtual void SwapBuffer() = 0;
    };
}

#endif //TK_ENGINE_TK_WINDOW_H
