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

    void MainController::draw_hens() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto lighting = engine::core::Controller::get<LightingController>();

        engine::resources::Model *hen = resources->model("hen");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();

        bool hens_crossed = lighting->hens_crossed();
        float crossing_progress = lighting->warning_active_progress();
        float turning_progress = lighting->warning_returning_progress();
        float negative_z_roadside = -6.5f;
        float positive_z_roadside = 4.7f;

        float first_start_z = hens_crossed ? positive_z_roadside : negative_z_roadside;
        float first_end_z = hens_crossed ? negative_z_roadside : positive_z_roadside;
        glm::vec3 first_start = glm::vec3(-2.0f, -0.25f, first_start_z);
        glm::vec3 first_end = glm::vec3(-2.0f, -0.25f, first_end_z);
        glm::vec3 first_position = glm::mix(first_start, first_end, crossing_progress);
        float first_rotation = glm::radians(180.0f) * ((hens_crossed ? 1.0f : 0.0f) + turning_progress);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, first_position);
        model = glm::rotate(model, first_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f));
        shader->set_mat4("model", model);
        hen->draw(shader);

        glm::vec3 second_start = glm::vec3(2.0f, -0.25f, first_end_z);
        glm::vec3 second_end = glm::vec3(2.0f, -0.25f, first_start_z);
        glm::vec3 second_position = glm::mix(second_start, second_end, crossing_progress);
        float second_rotation = glm::radians(180.0f) * ((hens_crossed ? 0.0f : 1.0f) + turning_progress);

        model = glm::mat4(1.0f);
        model = glm::translate(model, second_position);
        model = glm::rotate(model, second_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f));
        shader->set_mat4("model", model);
        hen->draw(shader);
    }

    void MainController::draw_forest() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto tree = resources->model("tree");
        auto shader = resources->shader("basic");
        shader->use();

        struct TreeTransform {
            glm::vec3 position;
            float scale;
            float rotation;
        };

        const TreeTransform tree_transforms[] = {
            {glm::vec3(-6.5f, -1.35f, -7.5f), 0.015f, -25.0f},
            {glm::vec3(0.0f, -1.35f, -8.0f), 0.013f, 35.0f},
            {glm::vec3(6.5f, -1.35f, -7.0f), 0.017f, 70.0f},
            {glm::vec3(-7.0f, -1.35f, 6.0f), 0.014f, -55.0f},
            {glm::vec3(0.5f, -1.35f, 7.0f), 0.016f, 15.0f},
            {glm::vec3(7.0f, -1.35f, 6.0f), 0.013f, 100.0f},
        };

        for (const auto &tree_transform: tree_transforms) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, tree_transform.position);
            model = glm::rotate(model, glm::radians(tree_transform.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(tree_transform.scale));
            shader->set_mat4("model", model);
            tree->draw(shader);
        }
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

    void MainController::draw_grass() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto grass = resources->model("grass");
        auto shader = resources->shader("basic");
        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.35f, -1.0f));
        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
        shader->set_mat4("model", model);
        grass->draw(shader);
    }

    void MainController::draw_lamp_posts() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto lighting = engine::core::Controller::get<LightingController>();

        auto lamp_post = resources->model("lamp_post");
        auto shader = resources->shader("basic");
        shader->use();

        for (const auto &lamp_post_position: lighting->lamp_post_positions()) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lamp_post_position);
            model = glm::scale(model, glm::vec3(0.2f));
            shader->set_mat4("model", model);
            lamp_post->draw(shader);
        }

        auto lamp_light = resources->model("lamp_light");
        auto light_shader = resources->shader("light_source");
        light_shader->use();
        light_shader->set_mat4("projection", graphics->projection_matrix());
        light_shader->set_mat4("view", graphics->camera()->view_matrix());
        light_shader->set_vec3("color", lighting->current_point_light_color());

        for (const auto &lamp_post_position: lighting->lamp_post_positions()) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lighting->point_light_position(lamp_post_position));
            model = glm::scale(model, glm::vec3(0.25f));
            light_shader->set_mat4("model", model);
            lamp_light->draw(light_shader);
        }
    }

    void draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("night_skybox");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
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

        draw_hens();
        draw_forest();
        draw_grass();
        draw_road();
        draw_lamp_posts();
        draw_skybox();
    }
}
