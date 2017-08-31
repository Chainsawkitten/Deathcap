#pragma once

class Entity;
namespace Video {
    class ShaderProgram;
}
namespace Component {
    class Mesh;
}

#include <glm/glm.hpp>

/// Render program to render an entity using skin shader program.
class SkinRenderProgram {
    public:
        /// Create new default render program.
        /**
         * @param shaderProgram A GLSL shader program.
         */
        SkinRenderProgram(Video::ShaderProgram* shaderProgram);
        
        /// Destructor.
        ~SkinRenderProgram();
        
        /// Bind render program.
        /**
         * @param camera Camera to render from.
         * @param screenSize Screen size in pixels (width, height).
         */
        void PreRender(Entity* camera, const glm::vec2& screenSize);

        /// Render mesh.
        /**
         * @param mesh Mesh to render.
         */
        void Render(Component::Mesh* mesh) const;

        /// Unbind render program.
        void PostRender() const;
        
    private:
        Video::ShaderProgram* shaderProgram;

        glm::mat4 viewMat;
        glm::mat4 projectionMat;
        glm::mat4 viewProjectionMat;
};
