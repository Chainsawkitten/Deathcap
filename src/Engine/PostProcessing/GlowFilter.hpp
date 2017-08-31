#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>

namespace Video {
    class Shader;
}

/// Glow filter (second pass).
/**
 * Applies the blurred glow map to the screen.
 */
class GlowFilter : public Filter {
    public:
        /// Create new color filter.
        GlowFilter();
        
        /// Free allocated resources.
        ~GlowFilter();
        
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
};
