#include <LightingController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    void LightingController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_F).state() == engine::platform::Key::State::JustPressed) {
            m_flashlight_enabled = !m_flashlight_enabled;
        }
    }

    void LightingController::set_lighting_uniforms(const engine::resources::Shader *shader) const {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();

        shader->use();
        shader->set_vec3("directionalLight.direction", glm::vec3(-0.45f, -1.0f, 0.7f));
        shader->set_vec3("directionalLight.ambient", glm::vec3(0.12f, 0.14f, 0.20f));
        shader->set_vec3("directionalLight.diffuse", glm::vec3(0.30f, 0.34f, 0.46f));
        shader->set_vec3("directionalLight.specular", glm::vec3(0.05f, 0.06f, 0.08f));
        shader->set_vec3("viewPosition", camera->Position);

        shader->set_vec3("pointLight.position", point_light_position());
        shader->set_float("pointLight.constant", 1.0f);
        shader->set_float("pointLight.linear", 0.09f);
        shader->set_float("pointLight.quadratic", 0.032f);
        shader->set_vec3("pointLight.ambient", m_point_light_color * 0.05f);
        shader->set_vec3("pointLight.diffuse", m_point_light_color);
        shader->set_vec3("pointLight.specular", m_point_light_color * 0.15f);

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
