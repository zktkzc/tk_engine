#ifndef TK_ENGINE_TK_GLFW_WINDOW_H
#define TK_ENGINE_TK_GLFW_WINDOW_H

#include "TkWindow.h"
#include "GLFW/glfw3.h"

namespace tk {
    class TkGLFWWindow : public TkWindow {
    public:
        TkGLFWWindow() = delete;

        TkGLFWWindow(uint32_t width, uint32_t height, const char *title);

        ~TkGLFWWindow() override;

        bool ShouldClose() override;

        void PollEvents() override;

        void SwapBuffer() override;

    private:
        GLFWwindow *mGLFWWindow;
    };
}

#endif //TK_ENGINE_TK_GLFW_WINDOW_H
