#include <iostream>
#include "TkLog.h"
#include "TkWindow.h"
#include "TkGraphicContext.h"
#include "Graphic/TkVKGraphicContext.h"
#include "Graphic/TkVKDevice.h"

int main() {
    tk::TkLog::Init();
    LOG_T("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_D("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_I("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_W("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    LOG_E("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);

    std::unique_ptr<tk::TkWindow> window = tk::TkWindow::Create(800, 600, "SandBox");
    std::unique_ptr<tk::TkGraphicContext> graphicContext = tk::TkGraphicContext::Create(window.get());
    std::shared_ptr<tk::TkVKDevice> device = std::make_shared<tk::TkVKDevice>(dynamic_cast<tk::TkVKGraphicContext *>(graphicContext.get()), 1, 1);

    while (!window->ShouldClose()) {
        window->PollEvents();
        window->SwapBuffer();
    }
    return EXIT_SUCCESS;
}