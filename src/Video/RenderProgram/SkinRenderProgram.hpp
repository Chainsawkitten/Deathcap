#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "RenderProgram.hpp"

namespace Video {
    class Texture2D;
    class ShaderProgram;
    class StorageBuffer;
    namespace Geometry {
        class Geometry3D;
    }
    
    /// Render program to render an entity using skin shader program.
    class SkinRenderProgram : public RenderProgram {
        public:
            /// Create new skin render program.
            SkinRenderProgram();
            
            /// Destructor.
            ~SkinRenderProgram() final;
            
            /// Bind shadow render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param shadowId The id for the shadowtexture.
             * @param shadowWidth The width of the shadowmap,
             * @param shadowHeight The height of the shadowmap.
             * @param depthFbo The framebufferobject containing the shadowmap.
             */
            //void PreShadowRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int shadowId, int shadowWidth, int shadowHeight, int depthFbo);

            /// Bind depth render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
            */
            //void PreDepthRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

            /// Render depth pass.
            /**
             * @param geometry The geometry to render.
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param modelMatrix Model matrix.
             */
            //void DepthRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix) const;

            /// Render shadow pass.
            /**
             * @param geometry The geometry to render.
             * @param viewMatrix The lights's view matrix.
             * @param projectionMatrix The lights's projection matrix.
             * @param modelMatrix Model matrix.
             */
            //void ShadowRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix) const;

            /// Bind render program.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param lightBuffer %StorageBuffer containing light data.
             * @param lightCount Number of lights in the light buffer.
             */
            void PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const StorageBuffer* lightBuffer, unsigned int lightCount);
    
            /// Render skinned geometry.
            /**
             * @param geometry The geometry to render.
             * @param textureAlbedo Albedo texture.
             * @param normalTexture Normal texture.
             * @param textureMetallic Metallic texture.
             * @param textureRoughness Roughness texture.
             * @param modelMatrix Model matrix.
             * @param isSelected Whether mesh should be highlighted.
             * @param bones Transformations of skeleton.
             */
            void Render(const Video::Geometry::Geometry3D* geometry, const Video::Texture2D* textureAlbedo, const Video::Texture2D* normalTexture, const Video::Texture2D* textureMetallic, const Video::Texture2D* textureRoughness, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones, bool isSelected) const;
            
        private:
            SkinRenderProgram(const SkinRenderProgram & other) = delete;
            //ShaderProgram* shadowProgram;
            //ShaderProgram* zShaderProgram;
            ShaderProgram* shaderProgram;

            //bool first = true;

            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::mat4 viewProjectionMatrix;
           // glm::mat4 lightSpaceMatrix;

            //int shadowId = 0;
    };
}
