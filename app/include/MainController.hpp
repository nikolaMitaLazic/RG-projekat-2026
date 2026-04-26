//
// Created by root on 4/26/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void draw_hen();

        void draw_tree();

        void begin_draw() override;

        void end_draw() override;

        void draw() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };
}

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP
