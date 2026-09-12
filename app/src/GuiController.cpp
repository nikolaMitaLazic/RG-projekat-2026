#include "GuiController.hpp"

#include "imgui.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"

#include "LightingController.hpp"
#include "VignetteController.hpp"

namespace app {
    void GuiController::initialize() {
        set_enable(false);
    }

    void GuiController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
    }

    void GuiController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto lighting = engine::core::Controller::get<LightingController>();
        auto vignette = engine::core::Controller::get<VignetteController>();
        auto camera = graphics->camera();
        graphics->begin_gui();

        ImGui::Begin("Info");

        ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);
        glm::vec3 point_light_color = lighting->point_light_color();
        if (ImGui::ColorEdit3("Lamp color", &point_light_color.x)) {
            lighting->set_point_light_color(point_light_color);
        }
        glm::vec3 warning_color = lighting->warning_color();
        if (ImGui::ColorEdit3("Warning color", &warning_color.x)) {
            lighting->set_warning_color(warning_color);
        }
        float flashlight_strength = lighting->flashlight_strength();
        if (ImGui::SliderFloat("Flash power", &flashlight_strength, 0.0f, 2.0f)) {
            lighting->set_flashlight_strength(flashlight_strength);
        }
        float vignette_strength = vignette->vignette_strength();
        if (ImGui::SliderFloat("Vignette strength", &vignette_strength, 0.0f, 1.0f)) {
            vignette->set_vignette_strength(vignette_strength);
        }
        if (lighting->is_crossing_in_progress())
            ImGui::Text("Crossing in progress...");
        else
            ImGui::Text("C: Trigger crossing");
        ImGui::Text("F: Toggle flashlight");
        ImGui::Text("P: Toggle post-processing");
        ImGui::Text("Esc: Close program");
        ImGui::End();
        graphics->end_gui();
    }
} // app
