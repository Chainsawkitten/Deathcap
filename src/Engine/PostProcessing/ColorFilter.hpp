#pragma once

#include <Video/PostProcessing/Filter.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Video {
    class Shader;
}

/// Color adjustment filter.
class ColorFilter : public Video::Filter {
    public:
        /// Create new color filter.
        /**
         * @param color Color.
         */
        ColorFilter(const glm::vec3& color);
        
        /// Free allocated resources.
        ~ColorFilter();
        
        /// Set color.
        /**
         * @param color Color.
         */
        void SetColor(const glm::vec3& color);
        
        /// Get shader program.
        /**
         * @return Shader program
         */
        Video::ShaderProgram* GetShaderProgram() const;
        
        /// Set uniforms.
        void SetUniforms();
        
    private:
        Video::Shader* vertexShader;
        Video::Shader* fragmentShader;
        Video::ShaderProgram* shaderProgram;
        
        glm::vec3 color;
        GLint colorLocation;
};
