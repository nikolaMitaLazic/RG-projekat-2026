#include <PostProcessingController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>

namespace app {
    void PostProcessingController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        m_post_processor.initialize(platform->window()->width(), platform->window()->height());
    }

    void PostProcessingController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_P).state() == engine::platform::Key::State::JustPressed) {
            m_effect_enabled = !m_effect_enabled;
        }
    }

    void PostProcessingController::begin_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        m_post_processor.resize(platform->window()->width(), platform->window()->height());
        m_post_processor.begin_capture();
    }

    void PostProcessingController::draw() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto shader = resources->shader("post_processing");
        shader->use();
        shader->set_bool("effectEnabled", m_effect_enabled);
        shader->set_float("vignetteStrength", m_vignette_strength);
        m_post_processor.present(shader);
    }

    void PostProcessingController::terminate() {
        m_post_processor.destroy();
    }
}
