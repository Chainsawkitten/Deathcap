#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Video {
    class Shader;
}
namespace Component {
    class Lens;
}

/// Post-process fog filter.
class FogFilter : public Filter {
    public:
        /// Create new fog filter.
        /**
         * @param color Fog color.
         */
        FogFilter(const glm::vec3& color);
        
        /// Free allocated resources.
        ~FogFilter();
        
        /// Set color.
        /**
         * @param color Color.
         */
        void SetColor(const glm::vec3& color);
        
        /// Set density.
        /**
         * @param density The density of the fog.
         */
        void SetDensity(float density);
        
        /// Set camera.
        /**
         * @param lens %Lens.
         */
        void SetCamera(const Component::Lens* lens);
        
        /// Set the screen size used when calculating fog.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        void SetScreenSize(const glm::vec2& screenSize);
        
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
        
        const Component::Lens* lens = nullptr;
        glm::vec2 screenSize;
        GLint matrixLocation;
        
        float density;
        GLint densityLocation;
};
