#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Video {
    struct Light;
    class Lighting;
    class StaticRenderProgram;
    class SkinRenderProgram;
    class Texture;
    class PostProcessing;
    class ColorFilter;
    class FogFilter;
    class FXAAFilter;
    class GammaCorrectionFilter;
    class GlowBlurFilter;
    class GlowFilter;
    namespace Geometry {
        class Geometry3D;
    }
    
    /// Handles rendering using OpenGL.
    class Renderer {
        public:
            /// Create new renderer.
            /**
             * @param screenSize Size of the screen in pixels.
             */
            Renderer(const glm::vec2& screenSize);
            
            /// Destructor.
            ~Renderer();
            
            /// Set screen size.
            /**
             * @param screenSize Size of the screen in pixels.
             */
            void SetScreenSize(const glm::vec2& screenSize);
            
            /// Clear the previous frame's data.
            void Clear();
            
            /// Start rendering the frame.
            void StartRendering();
            
            /// Prepare for rendering static meshes.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PrepareStaticMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
            
            /// Render a static mesh.
            /**
             * @param geometry The geometry to render.
             * @param diffuseTexture Diffuse texture.
             * @param normalTexture Normal map.
             * @param specularTexture Specular map.
             * @param glowTexture Glow texture.
             * @param modelMatrix Model matrix.
             */
            void RenderStaticMesh(Geometry::Geometry3D* geometry, const Texture* diffuseTexture, const Texture* normalTexture, const Texture* specularTexture, const Texture* glowTexture, const glm::mat4 modelMatrix);
            
            /// Prepare for rendering skinned meshes.
            /**
             * @param viewMatrix The camera's view matrix.
             * @param projectionMatrix The camera's projection matrix.
             */
            void PrepareSkinnedMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
            
            /// Render a skinned mesh.
            /**
             * @param geometry The geometry to render.
             * @param diffuseTexture Diffuse texture.
             * @param normalTexture Normal map.
             * @param specularTexture Specular map.
             * @param glowTexture Glow texture.
             * @param modelMatrix Model matrix.
             * @param bones Transformations of skeleton.
             * @param bonesIT Inverse transpose transformations of skeleton.
             */
            void RenderSkinnedMesh(const Video::Geometry::Geometry3D* geometry, const Video::Texture* diffuseTexture, const Video::Texture* normalTexture, const Video::Texture* specularTexture, const Video::Texture* glowTexture, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones, const std::vector<glm::mat3>& bonesIT);
            
            /// Add a light to the scene.
            void AddLight(const Video::Light& light);
            
            /// Light the scene with the added lights.
            /**
             * @param inverseProjectionMatrix The camera's inverse projection matrix.
             */
            void Light(const glm::mat4& inverseProjectionMatrix);
            
            /// Anti-alias using FXAA.
            void AntiAlias();
            
            /// Render fog.
            /**
             * @param projectionMatrix The camera's projection matrix.
             * @param density The density of the fog.
             * @param color Color.
             */
            void RenderFog(const glm::mat4& projectionMatrix, float density, const glm::vec3& color);
            
            /// Apply glow effect.
            /**
             * @param blurAmount How many times to blur the glow buffer.
             */
            void ApplyGlow(int blurAmount);
            
            /// Apply a color filter.
            /**
             * @param color Color.
             */
            void ApplyColorFilter(const glm::vec3& color);
            
            /// Perform gamma correction.
            void GammaCorrect();
            
            /// Display the rendered results.
            /**
             * @param dither Whether to use dithering.
             */
            void DisplayResults(bool dither);
            
            
        private:
            glm::vec2 screenSize;
            Lighting* lighting;
            StaticRenderProgram* staticRenderProgram;
            SkinRenderProgram* skinRenderProgram;
            
            PostProcessing* postProcessing;
            ColorFilter* colorFilter;
            FogFilter* fogFilter;
            FXAAFilter* fxaaFilter;
            GammaCorrectionFilter* gammaCorrectionFilter;
            GlowFilter* glowFilter;
            GlowBlurFilter* glowBlurFilter;
    };
}
