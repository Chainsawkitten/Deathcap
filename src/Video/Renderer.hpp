#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Lighting/Light.hpp"
#include "linking.hpp"

namespace Video {
    class StaticRenderProgram;
    class SkinRenderProgram;
    class Texture2D;
    class PostProcessing;
    class FXAAFilter;
    class ShaderProgram;
    class RenderSurface;
    class FrameBuffer;
    class StorageBuffer;
    namespace Geometry {
        class Geometry3D;
        class Rectangle;
    } // namespace Geometry

    /// Handles rendering using OpenGL.
    class Renderer {
        public:
        /// Create new renderer.
        VIDEO_API Renderer();

        /// Destructor.
        VIDEO_API ~Renderer();

        ////Prepare for shadow renderin
        /**
            * @param lightView The lights view matrix
            * @param lightProjection the Lights projection matrix
            * @param shadowId the texture ID for the shadowMap
            * @param shadowWidth size x of shadowMap
            * @param shadowHeight size y of shadowMap
            * @param depthFbo framebuffer object for shadowMap
            */
        VIDEO_API void PrepareShadowRendering(const glm::mat4 lightView, glm::mat4 lightProjection, int shadowId, int shadowWidth, int shadowHeight, int dephtFbo);

        /// Prepare for depth rendering static meshes.
        /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
        VIDEO_API void PrepareStaticMeshDepthRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

        /// Render a static mesh.
        /**
             * @param geometry The geometry to render.
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param modelMatrix Model matrix.
             */
        VIDEO_API void DepthRenderStaticMesh(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix);

        /// Render a Shadow mesh.
        /**
            * @param geometry The geometry to render.
            * @param viewMatrix The camera's view matrix.
            * @param projectionMatrix The camera's projection matrix.
            * @param modelMatrix Model matrix.
            */
        VIDEO_API void ShadowRenderStaticMesh(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix);

        /// Start rendering the frame.
        /**
             * @param renderSurface %RenderSurface to render to.
             */
        VIDEO_API void StartRendering(RenderSurface* renderSurface);

        /// Prepare for rendering static meshes.
        /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
        VIDEO_API void PrepareStaticMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

        /// Render a static mesh.
        /**
             * @param geometry The geometry to render.
             * @param albedo Albedo texture.
             * @param normal Normal map.
             * @param metallic Metallic map.
             * @param roughness Roughness texture.
             * @param modelMatrix Model matrix.
             * @param isSelected Whether model is selected(should be highlighted) or not.
             */
        VIDEO_API void RenderStaticMesh(Geometry::Geometry3D* geometry, const Texture2D* albedo, const Texture2D* normal, const Texture2D* metallic, const Texture2D* roughness, const glm::mat4 modelMatrix, bool isSelected);

        /// Update light buffer.
        /**
             * @param lights Vector of lights to push to the light buffer.
             */
        VIDEO_API void SetLights(const std::vector<Video::Light>& lights);

        /// Anti-alias using FXAA.
        /**
             * @param renderSurface %RenderSurface to apply filter to.
             */
        VIDEO_API void AntiAlias(RenderSurface* renderSurface);

        /// Render fog.
        /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param projectionMatrix The camera's projection matrix.
             * @param density The density of the fog.
             * @param color Color.
             */
        VIDEO_API void RenderFog(RenderSurface* renderSurface, const glm::mat4& projectionMatrix, float density, const glm::vec3& color);

        /// Apply glow effect.
        /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param blurAmount How many times to blur the glow buffer.
             */
        VIDEO_API void ApplyGlow(RenderSurface* renderSurface, int blurAmount);

        /// Apply a color filter.
        /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param color Color.
             */
        VIDEO_API void ApplyColorFilter(RenderSurface* renderSurface, const glm::vec3& color);

        /// Display the rendered results to back buffer.
        /**
             * @param renderSurface %RenderSurface to present to back buffer.
             */
        VIDEO_API void Present(RenderSurface* renderSurface);

        /// Begin rendering icons.
        /**
             * Needs to be called before RenderIcon.
             * @param viewProjectionMatrix The camera's view projection matrix.
             * @param cameraPosition The camera's position.
             * @param cameraUp The camera's up vector.
             */
        VIDEO_API void PrepareRenderingIcons(const glm::mat4& viewProjectionMatrix, const glm::vec3& cameraPosition, const glm::vec3& cameraUp);

        /// Render a billboarded icon.
        /**
             * PrepareRenderingIcons must be called before.
             * @param position World position to render at.
             * @param icon The icon to render.
             */
        VIDEO_API void RenderIcon(const glm::vec3& position, const Texture2D* icon);

        /// Stop rendering icons.
        /**
             * Should be called after all icons have been rendered.
             */
        VIDEO_API void StopRenderingIcons();

        private:
        Renderer(const Renderer& other) = delete;
        StaticRenderProgram* staticRenderProgram;

        unsigned int lightCount;
        StorageBuffer* lightBuffer;

        PostProcessing* postProcessing;
        FXAAFilter* fxaaFilter;

        // Icon rendering.
        ShaderProgram* iconShaderProgram;
        GLuint vertexBuffer;
        GLuint vertexArray;
        const Texture2D* currentIcon = nullptr;

        Geometry::Rectangle* rectangle;
    };
} // namespace Video
