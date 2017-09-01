#pragma once

#include <Video/PostProcessing/Filter.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Video {
    class Shader;
}

/// Glow filter (first pass).
/**
 * Blurs the glow buffer along a specified axis.
 */
class GlowBlurFilter : public Video::Filter {
    public:
        /// Create new color filter.
        GlowBlurFilter();
        
        /// Free allocated resources.
        ~GlowBlurFilter();
        
        /// Get shader program.
        /**
         * @return Shader program
         */
        Video::ShaderProgram* GetShaderProgram() const;
        
        /// Set uniforms.
        void SetUniforms();
        
        /// Set the screen size used when blurring.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        void SetScreenSize(const glm::vec2& screenSize);
        
        /// Set axis to blur.
        /**
         * @param horizontal Whether the blur should be horizontal (otherwise vertical).
         */
        void SetHorizontal(bool horizontal);
        
    private:
        Video::Shader* vertexShader;
        Video::Shader* fragmentShader;
        Video::ShaderProgram* shaderProgram;
        
        glm::vec2 screenSize;
        GLint screenSizeLocation;
        
        float horizontal;
        GLint horizontalLocation;
};
