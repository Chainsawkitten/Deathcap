#pragma once

#include <glm/glm.hpp>

namespace Video {
    class Texture2D;
    class ShaderProgram;
    class StorageBuffer;
    namespace Geometry {
        class Geometry3D;
    }

    /// Render program to render an entity using default shader program.
    class StaticRenderProgram {
        public:
            /// Create new default render program.
            StaticRenderProgram();

            /// Destructor.
            ~StaticRenderProgram();
            /// Bind shadow render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PreShadowRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int shadowId, int shadowWidth, int shadowHeight, int depthFbo);

            /// Bind depth render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PreDepthRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

            /// Render depth pass.
            /**
             * @param geometry The geometry to render.
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param modelMatrix Model matrix.
             */
            void DepthRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4 modelMatrix) const;

            /// Render shadow pass.
            /**
             * @param geometry The geometry to render.
             * @param viewMatrix The lights's view matrix.
             * @param projectionMatrix The lights's projection matrix.
             * @param modelMatrix Model matrix.
             */
            void ShadowRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4 &modelMatrix) const;

            /// Bind render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param lightBuffer %StorageBuffer containing light data.
             * @param lightCount Number of lights in the light buffer.
             */
            void PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const StorageBuffer* lightBuffer, unsigned int lightCount);

            /// Render mesh.
            /**
             * @param geometry The geometry to render.
             * @param textureAlbedo Albedo texture.
             * @param normalTexture Normal texture.
             * @param textureMetallic Metallic texture.
             * @param textureRoughness Roughness texture.
             * @param modelMatrix Model matrix.
                 */
            void Render(Geometry::Geometry3D* geometry, const Video::Texture2D* textureAlbedo, const Video::Texture2D* normalTexture, const Video::Texture2D* textureMetallic, const Video::Texture2D* textureRoughness, const glm::mat4 modelMatrix, bool isSelected) const;

        private:
            StaticRenderProgram(const StaticRenderProgram& other) = delete;
            ShaderProgram* shaderProgram;
            ShaderProgram* zShaderProgram;
            ShaderProgram* shadowProgram;

            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::mat4 viewProjectionMatrix;
            glm::mat4 lightSpaceMatrix;
            int shadowId;
    };
} // namespace Video
