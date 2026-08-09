#ifndef MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
#define MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>
#include <vector>

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

        const std::vector<glm::vec3> &lamp_post_positions() const {
            return m_lamp_post_positions;
        }

        glm::vec3 point_light_position(const glm::vec3 &lamp_post_position) const {
            return lamp_post_position + glm::vec3(0.0f, 3.34f, 0.0f);
        }

        glm::vec3 point_light_color() const {
            return m_point_light_color;
        }

        void set_point_light_color(const glm::vec3 &color) {
            m_point_light_color = color;
        }

        glm::vec3 warning_color() const {
            return m_warning_color;
        }

        void set_warning_color(const glm::vec3 &color) {
            m_warning_color = color;
        }

        glm::vec3 current_point_light_color() const {
            if (m_warning_state == WarningState::Waiting || m_warning_state == WarningState::Returning) {
                return glm::vec3(0.0f);
            }
            return is_warning_active() ? m_warning_color : m_point_light_color;
        }

        bool is_warning_active() const {
            return m_warning_state == WarningState::Active;
        }

    private:
        enum class WarningState {
            Idle,
            Waiting,
            Active,
            Returning
        };

        static constexpr float WARNING_TRANSITION_DURATION = 0.5f;
        static constexpr float WARNING_DURATION = 3.0f;
        static constexpr float WARNING_STRENGTH = 2.5f;

        void poll_events() override;

        void update() override;

        bool m_flashlight_enabled = false;
        float m_flashlight_strength = 1.0f;
        std::vector<glm::vec3> m_lamp_post_positions = {
                glm::vec3(3.0f, -1.5f, 2.8f),
                glm::vec3(-3.0f, -1.5f, -4.8f),
        };
        glm::vec3 m_point_light_color = glm::vec3(1.0f, 0.8f, 0.45f);
        glm::vec3 m_warning_color = glm::vec3(1.0f, 0.1f, 0.0f);
        WarningState m_warning_state = WarningState::Idle;
        float m_warning_timer = 0.0f;
    };
}

#endif//MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
