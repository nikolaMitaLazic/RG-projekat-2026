#include <LightingController.hpp>
#include <algorithm>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Shader.hpp>
#include <string>

namespace app {
    void LightingController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_F).state() == engine::platform::Key::State::JustPressed) {
            m_flashlight_enabled = !m_flashlight_enabled;
        }
        if (platform->key(engine::platform::KEY_C).state() == engine::platform::Key::State::JustPressed
            && m_warning_state == WarningState::Idle) {
            m_warning_state = WarningState::Waiting;
            m_warning_timer = 0.0f;
        }
    }

    void LightingController::update() {
        if (m_warning_state == WarningState::Idle) return;

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        m_warning_timer += platform->dt();

        if (m_warning_timer >= 2.0f * WARNING_TRANSITION_DURATION + WARNING_DURATION) {
            m_warning_state = WarningState::Idle;
            m_warning_timer = 0.0f;
            m_hens_crossed = !m_hens_crossed;
        } else if (m_warning_timer >= WARNING_TRANSITION_DURATION + WARNING_DURATION) {
            m_warning_state = WarningState::Returning;
        } else if (m_warning_timer >= WARNING_TRANSITION_DURATION) {
            m_warning_state = WarningState::Active;
        }
    }

    float LightingController::warning_active_progress() const {
        return std::clamp((m_warning_timer - WARNING_TRANSITION_DURATION) / WARNING_DURATION, 0.0f, 1.0f);
    }

    float LightingController::warning_returning_progress() const {
        float returning_start = WARNING_TRANSITION_DURATION + WARNING_DURATION;
        return std::clamp((m_warning_timer - returning_start) / WARNING_TRANSITION_DURATION, 0.0f, 1.0f);
    }

    void LightingController::set_lighting_uniforms(const engine::resources::Shader *shader) const {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        glm::vec3 point_light_color = current_point_light_color();
        float point_light_strength = is_warning_active() ? WARNING_STRENGTH : 1.0f;

        shader->use();
        shader->set_vec3("directionalLight.direction", glm::vec3(-0.45f, -1.0f, 0.7f));
        shader->set_vec3("directionalLight.ambient", glm::vec3(0.12f, 0.14f, 0.20f));
        shader->set_vec3("directionalLight.diffuse", glm::vec3(0.30f, 0.34f, 0.46f));
        shader->set_vec3("directionalLight.specular", glm::vec3(0.05f, 0.06f, 0.08f));
        shader->set_vec3("viewPosition", camera->Position);

        for (std::size_t i = 0; i < m_lamp_post_positions.size(); i++) {
            std::string uniform = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(uniform + "position", point_light_position(m_lamp_post_positions[i]));
            shader->set_float(uniform + "constant", 1.0f);
            shader->set_float(uniform + "linear", 0.09f);
            shader->set_float(uniform + "quadratic", 0.032f);
            shader->set_vec3(uniform + "ambient", point_light_color * 0.05f * point_light_strength);
            shader->set_vec3(uniform + "diffuse", point_light_color * point_light_strength);
            shader->set_vec3(uniform + "specular", point_light_color * 0.15f * point_light_strength);
        }

        shader->set_bool("flashlight.enabled", m_flashlight_enabled);
        shader->set_vec3("flashlight.position", camera->Position);
        shader->set_vec3("flashlight.direction", camera->Front);
        shader->set_float("flashlight.innerCutoff", glm::cos(glm::radians(12.5f)));
        shader->set_float("flashlight.outerCutoff", glm::cos(glm::radians(17.5f)));
        shader->set_float("flashlight.constant", 1.0f);
        shader->set_float("flashlight.linear", 0.09f);
        shader->set_float("flashlight.quadratic", 0.02f);
        shader->set_vec3("flashlight.ambient", glm::vec3(0.0f));
        shader->set_vec3("flashlight.diffuse", glm::vec3(1.1f, 1.1f, 0.95f) * m_flashlight_strength);
        shader->set_vec3("flashlight.specular", glm::vec3(0.2f, 0.2f, 0.18f) * m_flashlight_strength);
    }
}
