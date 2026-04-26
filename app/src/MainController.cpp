#include <MainController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MainController::initialize() {
        spdlog::info("MainController: Successful initialization!");
        engine::graphics::OpenGL::enable_depth_testing();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down())
            return false;
        else
            return true;
    }

    void MainController::draw_hen() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();

        engine::resources::Model *hen     = resources->model("hen");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        float t         = platform->frame_time().current;
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(-2.0f, -1.0f, -10.0f));
        model           = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
        model           = glm::rotate(model, -t, glm::vec3(0.0, 1.0, 0.0));
        shader->set_mat4("model", model);
        hen->draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -1.0f, -10.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
        model = glm::rotate(model, t, glm::vec3(0.0, 1.0, 0.0));
        shader->set_mat4("model", model);
        hen->draw(shader);
    }

    void MainController::draw_tree() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *tree    = resources->model("tree");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(0.0f, -2.0f, -10.0f));
        model           = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        shader->set_mat4("model", model);
        tree->draw(shader);
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::draw() {
        draw_hen();
        draw_tree();
    }
}
