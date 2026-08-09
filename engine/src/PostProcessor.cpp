// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/OpenGL.hpp>
#include <engine/graphics/PostProcessor.hpp>
#include <engine/resources/Shader.hpp>
#include <engine/util/Errors.hpp>

namespace engine::graphics {
void PostProcessor::initialize(int width, int height) {
    RG_GUARANTEE(width > 0 && height > 0, "Post-processing framebuffer dimensions must be positive.");

    CHECKED_GL_CALL(glGenFramebuffers, 1, &m_framebuffer);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_framebuffer);

    CHECKED_GL_CALL(glGenTextures, 1, &m_color_texture);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0);

    CHECKED_GL_CALL(glGenRenderbuffers, 1, &m_depth_stencil_renderbuffer);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, m_depth_stencil_renderbuffer);
    CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                    m_depth_stencil_renderbuffer);

    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, 0);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

    initialize_screen_quad();
    resize(width, height);
}

void PostProcessor::resize(int width, int height) {
    if (width <= 0 || height <= 0 || (width == m_width && height == m_height)) return;

    m_width = width;
    m_height = height;

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_framebuffer);

    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);
    CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, m_depth_stencil_renderbuffer);
    CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

    uint32_t framebuffer_status = CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER);

    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, 0);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

    RG_GUARANTEE(framebuffer_status == GL_FRAMEBUFFER_COMPLETE,
                 "Post-processing framebuffer is incomplete. OpenGL status: {}", framebuffer_status);
}

void PostProcessor::begin_capture() const {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_framebuffer);
    CHECKED_GL_CALL(glViewport, 0, 0, m_width, m_height);
    CHECKED_GL_CALL(glEnable, GL_DEPTH_TEST);
    OpenGL::clear_buffers();
}

void PostProcessor::present(const resources::Shader *shader) const {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    CHECKED_GL_CALL(glViewport, 0, 0, m_width, m_height);
    CHECKED_GL_CALL(glDisable, GL_DEPTH_TEST);
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);

    shader->use();
    shader->set_int("screenTexture", 0);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);
    CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);
    CHECKED_GL_CALL(glBindVertexArray, 0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
    CHECKED_GL_CALL(glEnable, GL_DEPTH_TEST);
}

void PostProcessor::destroy() {
    if (m_quad_vbo != 0) CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad_vbo);
    if (m_quad_vao != 0) CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad_vao);
    if (m_depth_stencil_renderbuffer != 0)
        CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_stencil_renderbuffer);
    if (m_color_texture != 0) CHECKED_GL_CALL(glDeleteTextures, 1, &m_color_texture);
    if (m_framebuffer != 0) CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_framebuffer);

    m_quad_vbo = 0;
    m_quad_vao = 0;
    m_depth_stencil_renderbuffer = 0;
    m_color_texture = 0;
    m_framebuffer = 0;
    m_width = 0;
    m_height = 0;
}

void PostProcessor::initialize_screen_quad() {
    // clang-format off
    const float quad_vertices[] = {
            // position      // texture coordinates
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
    };
    // clang-format on

    CHECKED_GL_CALL(glGenVertexArrays, 1, &m_quad_vao);
    CHECKED_GL_CALL(glGenBuffers, 1, &m_quad_vbo);
    CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_quad_vbo);
    CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
    CHECKED_GL_CALL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);// NOLINT
    CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
    CHECKED_GL_CALL(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                    (void *) (2 * sizeof(float)));// NOLINT

    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
    CHECKED_GL_CALL(glBindVertexArray, 0);
}
}// namespace engine::graphics
