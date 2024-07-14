#include <iostream>
#include "TkLog.h"
#include "TkWindow.h"

int main() {
    tk::TkLog::Init();
    LOG_T("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_D("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_I("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_W("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_E("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);

    std::unique_ptr<tk::TkWindow> window = tk::TkWindow::Create(800, 600, "SandBox");
    while (!window->ShouldClose()) {
        window->PollEvents();
        window->SwapBuffer();
    }
    return EXIT_SUCCESS;
}