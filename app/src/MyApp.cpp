#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>

#include "GuiController.hpp"

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, MyApp::app_setup");
        auto main_controller = register_controller<MainController>();
        auto gui_controller = register_controller<GuiController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->before(gui_controller);
    }
}


