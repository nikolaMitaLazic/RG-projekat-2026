#include <MainController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

#include "GuiController.hpp"
#include "LightingController.hpp"

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GuiController>();
        if (gui_controller->is_enabled()) return;
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
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

    void MainController::update_camera() {
        auto gui_controller = engine::core::Controller::get<GuiController>();
        if (gui_controller->is_enabled()) return;
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::draw_hen() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

        engine::resources::Model *hen = resources->model("hen");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();

        float t = platform->frame_time().current;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -0.25f, -1.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
        model = glm::rotate(model, -t, glm::vec3(0.0, 1.0, 0.0));
        shader->set_mat4("model", model);
        hen->draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.25f, 1.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
        model = glm::rotate(model, t, glm::vec3(0.0, 2.0, 0.0));
        shader->set_mat4("model", model);
        hen->draw(shader);
    }

    void MainController::draw_tree() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        engine::resources::Model *tree = resources->model("tree");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, -3.0f));
        model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        shader->set_mat4("model", model);
        tree->draw(shader);
    }

    void MainController::draw_road() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        engine::resources::Model *road = resources->model("road");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        shader->set_mat4("model", model);
        road->draw(shader);
    }

    void MainController::draw_lamp_post() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto lighting = engine::core::Controller::get<LightingController>();

        auto lamp_post = resources->model("lamp_post");
        auto shader = resources->shader("basic");
        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lighting->lamp_post_position());
        model = glm::scale(model, glm::vec3(0.2f));
        shader->set_mat4("model", model);
        lamp_post->draw(shader);

        auto lamp_light = resources->model("lamp_light");
        auto light_shader = resources->shader("light_source");
        light_shader->use();
        light_shader->set_mat4("projection", graphics->projection_matrix());
        light_shader->set_mat4("view", graphics->camera()->view_matrix());
        light_shader->set_vec3("color", lighting->current_point_light_color());

        model = glm::mat4(1.0f);
        model = glm::translate(model, lighting->point_light_position());
        model = glm::scale(model, glm::vec3(0.25f));
        light_shader->set_mat4("model", model);
        lamp_light->draw(light_shader);
    }

    void draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("night_skybox");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::draw() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        engine::core::Controller::get<LightingController>()->set_lighting_uniforms(shader);

        draw_hen();
        draw_tree();
        draw_road();
        draw_lamp_post();
        draw_skybox();
    }
}
