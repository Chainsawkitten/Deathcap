#pragma once

#include <glm/glm.hpp>

namespace Video {
    class Texture;
    class ShaderProgram;
    namespace Geometry {
        class Geometry3D;
    }
    
    /// Render program to render an entity using default shader program.
    class StaticRenderProgram {
        public:
            /// Create new default render program.
            /**
             * @param shaderProgram A GLSL shader program.
             */
            StaticRenderProgram(ShaderProgram* shaderProgram);
            
            /// Destructor.
            ~StaticRenderProgram();
            
            /// Bind render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    
            /// Render mesh.
            /**
             * @param geometry The geometry to render.
             * @param diffuseTexture Diffuse texture.
             * @param normalTexture Normal map.
             * @param specularTexture Specular map.
             * @param glowTexture Glow texture.
             * @param modelMatrix Model matrix.
             */
            void Render(Geometry::Geometry3D* geometry, const Texture* diffuseTexture, const Texture* normalTexture, const Texture* specularTexture, const Texture* glowTexture, const glm::mat4 modelMatrix) const;
    
            /// Unbind render program.
            void PostRender() const;
            
        private:
            ShaderProgram* shaderProgram;
    
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::mat4 viewProjectionMatrix;
    };
}
