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
    };
}

#endif//MATF_RG_PROJECT_LIGHTINGCONTROLLER_HPP
