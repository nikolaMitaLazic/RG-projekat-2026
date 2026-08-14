/**
 * @file PostProcessingController.hpp
 * @brief Defines the PostProcessingController class used for framebuffer post-processing.
 */

#ifndef MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP
#define MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP

#include <cstdint>
#include <engine/core/Controller.hpp>

namespace engine::resources {
    class Shader;
}

namespace engine::graphics {
    /**
     * @class PostProcessingController
     * @brief Controller that captures scene rendering into a texture for post-processing.
     */
    class PostProcessingController : public core::Controller {
    public:
        std::string_view name() const override {
            return "engine::graphics::PostProcessingController";
        }

    protected:
        /**
         * @brief Creates the framebuffer attachments and screen quad.
         */
        void initialize() override;

        /**
         * @brief Resizes and binds the framebuffer before scene drawing.
         */
        void begin_draw() override;

        /**
         * @brief Draws the captured scene to the default framebuffer.
         * @param shader Shader containing a sampler2D uniform named screenTexture.
         */
        void present(const resources::Shader *shader) const;

        void draw() override = 0;

        /**
         * @brief Destroys the framebuffer attachments and screen quad.
         */
        void terminate() override;

    private:
        void resize(int width, int height);

        void begin_capture() const;

        void destroy();

        void initialize_screen_quad();

        uint32_t m_framebuffer = 0;
        uint32_t m_color_texture = 0;
        uint32_t m_depth_stencil_renderbuffer = 0;
        uint32_t m_quad_vao = 0;
        uint32_t m_quad_vbo = 0;
        int m_width = 0;
        int m_height = 0;
    };
} // namespace engine::graphics

#endif//MATF_RG_PROJECT_POSTPROCESSINGCONTROLLER_HPP
