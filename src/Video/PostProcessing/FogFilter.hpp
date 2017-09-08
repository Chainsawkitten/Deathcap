#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Video {
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
            
            /// Set the camera's projection matrix.
            /**
             * @param projectionMatrix The camera's projection matrix.
             */
            void SetProjectionMatrix(const glm::mat4& projectionMatrix);
            
            /// Get shader program.
            /**
             * @return Shader program
             */
            ShaderProgram* GetShaderProgram() const;
            
            /// Set uniforms.
            void SetUniforms();
            
        private:
            ShaderProgram* shaderProgram;
            
            glm::vec3 color;
            GLint colorLocation;
            
            glm::mat4 projectionMatrix;
            GLint matrixLocation;
            
            float density;
            GLint densityLocation;
    };
}
