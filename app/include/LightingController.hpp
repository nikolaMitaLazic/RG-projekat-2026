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

        glm::vec3 lamp_post_position() const {
            return m_lamp_post_position;
        }

        glm::vec3 point_light_position() const {
            return m_lamp_post_position + glm::vec3(0.0f, 3.34f, 0.0f);
        }

    private:
        void poll_events() override;

        bool m_flashlight_enabled = false;
        glm::vec3 m_lamp_post_position = glm::vec3(3.0f, 0.5f, 2.8f);
    };
}

#endif//MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
