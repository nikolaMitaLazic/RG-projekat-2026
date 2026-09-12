#include <VignetteController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>

namespace app {
    void VignetteController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_P).state() == engine::platform::Key::State::JustPressed) {
            m_effect_enabled = !m_effect_enabled;
        }
    }

    void VignetteController::draw() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto shader = resources->shader("post_processing");
        shader->use();
        shader->set_bool("effectEnabled", m_effect_enabled);
        shader->set_float("vignetteStrength", m_vignette_strength);
        present(shader);
    }
}
