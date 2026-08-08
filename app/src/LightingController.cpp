#include <LightingController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    void LightingController::set_lighting_uniforms(const engine::resources::Shader *shader) const {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        shader->use();
        shader->set_vec3("directionalLight.direction", glm::vec3(-0.45f, -1.0f, 0.7f));
        shader->set_vec3("directionalLight.ambient", glm::vec3(0.18f, 0.20f, 0.28f));
        shader->set_vec3("directionalLight.diffuse", glm::vec3(0.45f, 0.50f, 0.65f));
        shader->set_vec3("directionalLight.specular", glm::vec3(0.30f, 0.35f, 0.45f));
        shader->set_vec3("viewPosition", graphics->camera()->Position);
    }
}
