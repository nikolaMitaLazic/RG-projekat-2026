#ifndef MATF_RG_PROJECT_VIGNETTECONTROLLER_HPP
#define MATF_RG_PROJECT_VIGNETTECONTROLLER_HPP

#include <engine/graphics/PostProcessingController.hpp>

namespace app {
    class VignetteController : public engine::graphics::PostProcessingController {
    public:
        std::string_view name() const override {
            return "app::VignetteController";
        }

        float vignette_strength() const {
            return m_vignette_strength;
        }

        void set_vignette_strength(float strength) {
            m_vignette_strength = strength;
        }

    private:
        void poll_events() override;

        void draw() override;

        bool m_effect_enabled = true;
        float m_vignette_strength = 0.45f;
    };
}

#endif//MATF_RG_PROJECT_VIGNETTECONTROLLER_HPP
