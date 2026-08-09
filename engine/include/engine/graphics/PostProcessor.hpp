/**
 * @file PostProcessor.hpp
 * @brief Defines the PostProcessor class used for framebuffer post-processing.
 */

#ifndef MATF_RG_PROJECT_POSTPROCESSOR_HPP
#define MATF_RG_PROJECT_POSTPROCESSOR_HPP

#include <cstdint>

namespace engine::resources {
    class Shader;
}

namespace engine::graphics {
    /**
     * @class PostProcessor
     * @brief Captures scene rendering into a texture and draws it through a post-processing shader.
     */
    class PostProcessor {
    public:
        /**
         * @brief Creates the framebuffer attachments and screen quad.
         * @param width Initial framebuffer width.
         * @param height Initial framebuffer height.
         */
        void initialize(int width, int height);

        /**
         * @brief Resizes the framebuffer attachments.
         * @param width New framebuffer width.
         * @param height New framebuffer height.
         */
        void resize(int width, int height);

        /**
         * @brief Binds and clears the framebuffer used to capture the scene.
         */
        void begin_capture() const;

        /**
         * @brief Draws the captured scene to the default framebuffer.
         * @param shader Shader containing a sampler2D uniform named screenTexture.
         */
        void present(const resources::Shader *shader) const;

        /**
         * @brief Destroys the framebuffer attachments and screen quad.
         */
        void destroy();

    private:
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

#endif//MATF_RG_PROJECT_POSTPROCESSOR_HPP
