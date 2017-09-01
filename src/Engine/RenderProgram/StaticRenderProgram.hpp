#pragma once

class Entity;
namespace Video {
    class ShaderProgram;
}
namespace Component {
    class Mesh;
}

#include <glm/glm.hpp>

/// Render program to render an entity using default shader program.
class StaticRenderProgram {
    public:
        /// Create new default render program.
        /**
         * @param shaderProgram A GLSL shader program.
         */
        StaticRenderProgram(Video::ShaderProgram* shaderProgram);
        
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
         * @param mesh Mesh to render.
         */
        void Render(Component::Mesh* mesh) const;

        /// Unbind render program.
        void PostRender() const;
        
    private:
        Video::ShaderProgram* shaderProgram;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;
};
