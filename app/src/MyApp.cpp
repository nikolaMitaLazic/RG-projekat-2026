#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>

#include "GuiController.hpp"
#include "LightingController.hpp"
#include "PostProcessingController.hpp"

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, MyApp::app_setup");
        auto lighting_controller = register_controller<LightingController>();
        auto main_controller = register_controller<MainController>();
        auto post_processing_controller = register_controller<PostProcessingController>();
        auto gui_controller = register_controller<GuiController>();
        lighting_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->after(lighting_controller);
        post_processing_controller->after(main_controller);
        post_processing_controller->before(gui_controller);
    }
}
