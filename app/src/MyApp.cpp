#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, TestApp::app_setup");
        auto main_controller = register_controller<MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    }
}


