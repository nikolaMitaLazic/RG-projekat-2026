#ifndef MATF_RG_PROJECT_GUICONTROLLER_H
#define MATF_RG_PROJECT_GUICONTROLLER_H
#include "engine/core/Controller.hpp"

namespace app {
    class GuiController : public engine::core::Controller {
        void initialize() override;

        void poll_events() override;

        void draw() override;

    public:
        std::string_view name() const override {
            return "app::GuiController";
        }
    };
} // app

#endif //MATF_RG_PROJECT_GUICONTROLLER_H
