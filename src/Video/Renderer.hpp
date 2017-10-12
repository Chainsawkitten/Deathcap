#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Lighting/Light.hpp"

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
    }
    
    /// Handles rendering using OpenGL.
    class Renderer {
        public:
            /// Create new renderer.
            Renderer();
            
            /// Destructor.
            ~Renderer(); 

            /// Prepare for depth rendering static meshes.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PrepareStaticMeshDepthRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

            /// Render a static mesh.
            /**
             * @param geometry The geometry to render.
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             * @param modelMatrix Model matrix.
             */
            void DepthRenderStaticMesh(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix);
            
            /// Start rendering the frame.
            /**
             * @param renderSurface %RenderSurface to render to.
             */
            void StartRendering(RenderSurface* renderSurface);
            
            /// Prepare for rendering static meshes.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PrepareStaticMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
            
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
            void RenderStaticMesh(Geometry::Geometry3D* geometry, const Texture2D* albedo, const Texture2D* normal, const Texture2D* metallic, const Texture2D* roughness, const glm::mat4 modelMatrix, bool isSelected);
            
            /// Add a light to the scene.
            /**
             * @param light The light to add.
             */
            void AddLight(const Video::Light& light);

            /// Clear lights in the scene.
            void ClearLights();
            
            /// Anti-alias using FXAA.
            /**
             * @param renderSurface %RenderSurface to apply filter to.
             */
            void AntiAlias(RenderSurface* renderSurface);
            
            /// Render fog.
            /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param projectionMatrix The camera's projection matrix.
             * @param density The density of the fog.
             * @param color Color.
             */
            void RenderFog(RenderSurface* renderSurface, const glm::mat4& projectionMatrix, float density, const glm::vec3& color);
            
            /// Apply glow effect.
            /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param blurAmount How many times to blur the glow buffer.
             */
            void ApplyGlow(RenderSurface* renderSurface, int blurAmount);
            
            /// Apply a color filter.
            /**
             * @param renderSurface %RenderSurface to apply filter to.
             * @param color Color.
             */
            void ApplyColorFilter(RenderSurface* renderSurface, const glm::vec3& color);
            
            /// Display the rendered results to back buffer.
            /**
             * @param renderSurface %RenderSurface to present to back buffer.
             */
            void Present(RenderSurface* renderSurface);
            
            /// Begin rendering icons.
            /**
             * Needs to be called before RenderIcon.
             * @param viewProjectionMatrix The camera's view projection matrix.
             * @param cameraPosition The camera's position.
             * @param cameraUp The camera's up vector.
             */
            void PrepareRenderingIcons(const glm::mat4& viewProjectionMatrix, const glm::vec3& cameraPosition, const glm::vec3& cameraUp);
            
            /// Render a billboarded icon.
            /**
             * PrepareRenderingIcons must be called before.
             * @param position World position to render at.
             * @param icon The icon to render.
             */
            void RenderIcon(const glm::vec3& position, const Texture2D* icon);
            
            /// Stop rendering icons.
            /**
             * Should be called after all icons have been rendered.
             */
            void StopRenderingIcons();
            
        private:
            Renderer(const Renderer & other) = delete;
            StaticRenderProgram* staticRenderProgram;

            std::vector<Video::Light> lights;
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
}
