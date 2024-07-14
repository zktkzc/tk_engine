#include "Window/TkGLFWWindow.h"
#include "TkLog.h"

namespace tk {
    TkGLFWWindow::TkGLFWWindow(uint32_t width, uint32_t height, const char *title) {
        if (!glfwInit()) {
            LOG_E("Failed to initialize GLFW");
            return;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE); // 默认隐藏窗口

        mGLFWWindow = glfwCreateWindow((int)width, (int)height, title, nullptr, nullptr);
        if (!mGLFWWindow) {
            LOG_E("Failed to create GLFW window");
            return;
        }

        // 使窗口居中显示
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if (primaryMonitor) {
            int xPos, yPos, workWidth, workHeight;
            glfwGetMonitorWorkarea(primaryMonitor, &xPos, &yPos, &workWidth, &workHeight);
            glfwSetWindowPos(mGLFWWindow, (int)(workWidth - width) / 2, (int)(workHeight - height) / 2);
        }

        glfwMakeContextCurrent(mGLFWWindow);

        // 显示窗口
        glfwShowWindow(mGLFWWindow);
    }

    TkGLFWWindow::~TkGLFWWindow() {
        glfwDestroyWindow(mGLFWWindow);
        glfwTerminate();
        LOG_I("The application running end.");
    }

    bool TkGLFWWindow::ShouldClose() {
        return glfwWindowShouldClose(mGLFWWindow);
    }

    void TkGLFWWindow::PollEvents() {
        glfwPollEvents();
    }

    void TkGLFWWindow::SwapBuffer() {
        glfwSwapBuffers(mGLFWWindow);
    }
}