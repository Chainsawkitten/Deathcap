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
             * @param shadowID The id for the shadowtexture.
             * @param shadowWidth The width of the shadowmap,
             * @param shadowHeight The height of the shadowmap.
             * @param depthFbo The framebufferobject containing the shadowmap.
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

            /// Gamma correction.
            /**
             * @param gamma Gamma to apply.
             */
            void SetGamma(float gamma);

            /// Gamma correction.
            /**
             * @return gamma Gamma value.
             */
            float GetGamma() const;


            /// Set whether fog is applied.
            /**
             * @param fogApply Whether to apply fog.
             */
            void SetFogApply(bool fogApply);

            /// Get whether fog is applied.
            /**
             * @return Whether fog is applied.
             */
            bool GetFogApply() const;


            /// Set fog density.
            /**
             * @param fogDensity Density of the fog.
             */
            void SetFogDensity(float fogDensity);

            /// get fog density.
            /**
             * @return Density of the fog
             */
            float GetFogDensity() const;


            /// Set fog color.
            /**
             * @param fogColor Color of the fog.
             */
            void SetFogColor(const glm::vec3& fogColor);

            /// Get fog color.
            /**
             * @return Color of the fog.
             */
            glm::vec3 GetFogColor() const;


            /// Set whether color filter is applied.
            /**
             * @param colorFilterApply Whether to apply color filter.
             */
            void SetColorFilterApply(bool colorFilterApply);

            /// Get whether color filter is applied.
            /**
             * @return Whether color filter is applied.
             */
            bool GetColorFilterApply() const;


            /// Set color filer color.
            /**
             * @param colorFilterColor Color of the color filter.
             */
            void SetColorFilterColor(const glm::vec3& colorFilterColor);

            /// Get color filer color.
            /**
             * @return Color of the color filter.
             */
            glm::vec3 GetColorFilterColor() const;


            /// Set whether dithering is applied.
            /**
             * @param ditherApply Whether to apply dithering.
             */
            void SetDitherApply(bool ditherApply);

            /// Get whether dithering is applied.
            /**
             * @return Whether dithering is applied.
             */
            bool GetDitherApply() const;


            /// Set size of the current frame to be rendered in pixels.
            /**
             * @param frameSize Size of the frame in pixels.
             */
            void SetFrameSize(const glm::vec2& frameSize);

            /// Get size of the current frame to be rendered in pixels.
            /**
             * @return Size of the frame in pixels.
             */
            glm::vec2 GetFrameSize() const;

        private:
            StaticRenderProgram(const StaticRenderProgram& other) = delete;
            ShaderProgram* shaderProgram;
            ShaderProgram* zShaderProgram;

            bool first = true;
      
            ShaderProgram* shadowProgram;
      
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::mat4 viewProjectionMatrix;
            glm::mat4 lightSpaceMatrix;

            int shadowId = 0;

            float gamma = 2.2f;
            int fogApply = false;
            float fogDensity = 0.002f;
            glm::vec3 fogColor = glm::vec3(1.f, 0.f, 0.f);
            int colorFilterApply = false;
            glm::vec3 colorFilterColor = glm::vec3(0, 1, 0);
            int ditherApply = true;
            glm::vec2 frameSize = glm::vec2(800.f, 800.f);
    };
} // namespace Video
