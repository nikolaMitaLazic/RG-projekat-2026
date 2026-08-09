#ifndef MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP
#define MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/graphics/PostProcessor.hpp>

namespace app {
    class PostProcessingController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::PostProcessingController";
        }

        float vignette_strength() const {
            return m_vignette_strength;
        }

        void set_vignette_strength(float strength) {
            m_vignette_strength = strength;
        }

    private:
        void initialize() override;

        void poll_events() override;

        void begin_draw() override;

        void draw() override;

        void terminate() override;

        engine::graphics::PostProcessor m_post_processor;
        bool m_effect_enabled = true;
        float m_vignette_strength = 0.45f;
    };
}

#endif//MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP
