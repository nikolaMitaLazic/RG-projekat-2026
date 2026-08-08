#ifndef MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
#define MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    class LightingController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::LightingController";
        }

        void set_lighting_uniforms(const engine::resources::Shader *shader) const;

        bool is_flashlight_enabled() const {
            return m_flashlight_enabled;
        }

        float flashlight_strength() const {
            return m_flashlight_strength;
        }

        void set_flashlight_strength(float strength) {
            m_flashlight_strength = strength;
        }

        glm::vec3 lamp_post_position() const {
            return m_lamp_post_position;
        }

        glm::vec3 point_light_position() const {
            return m_lamp_post_position + glm::vec3(0.0f, 3.34f, 0.0f);
        }

        glm::vec3 point_light_color() const {
            return m_point_light_color;
        }

        void set_point_light_color(const glm::vec3 &color) {
            m_point_light_color = color;
        }

    private:
        void poll_events() override;

        bool m_flashlight_enabled = false;
        float m_flashlight_strength = 1.0f;
        glm::vec3 m_lamp_post_position = glm::vec3(3.0f, -1.5f, 2.8f);
        glm::vec3 m_point_light_color = glm::vec3(1.0f, 0.8f, 0.45f);
    };
}

#endif//MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
