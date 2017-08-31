#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Video {
    class Renderer;
    class Shader;
    class ShaderProgram;
}
class World;
class Entity;
class StaticRenderProgram;
class SkinRenderProgram;
class DeferredLighting;
class PostProcessing;
class ColorFilter;
class FogFilter;
class FXAAFilter;
class GammaCorrectionFilter;
class GlowFilter;
class GlowBlurFilter;
class Texture2D;
namespace Component {
    class SuperComponent;
}

/// Handles rendering the world.
class RenderManager {
    friend class Hub;
    
    public:
        /// Render world containing entities.
        /**
         * @param world Contains a bunch of entities.
         * @param camera Camera through which to render (or first camera in the world if nullptr).
         */
        void Render(World& world, Entity* camera = nullptr);
        
        /// Render editor entities.
        /**
         * @param world World to render.
         * @param camera Camera through which to render (or first camera in the world if nullptr).
         * @param soundSources Whether to show sound sources.
         * @param particleEmitters Whether to show particle emitters.
         * @param lightSources Whether to show light sources.
         * @param cameras Whether to show cameras.
         */
        void RenderEditorEntities(World& world, Entity* camera = nullptr, bool soundSources = true, bool particleEmitters = true, bool lightSources = true, bool cameras = true);
        
        /// Updates the buffers to fit the current screen size.
        void UpdateBufferSize();
        
    private:
        RenderManager();
        ~RenderManager();
        RenderManager(RenderManager const&) = delete;
        void operator=(RenderManager const&) = delete;
        
        void RenderEditorEntity(Component::SuperComponent* component);

        Video::Renderer* renderer;
        
        Video::Shader* defaultVertexShader;
        Video::Shader* defaultFragmentShader;
        Video::Shader* skinningVertexShader;
        Video::ShaderProgram* staticShaderProgram;
        Video::ShaderProgram* skinShaderProgram;
        StaticRenderProgram* staticRenderProgram;
        SkinRenderProgram* skinRenderProgram;
        
        Video::Shader* editorEntityVertexShader;
        Video::Shader* editorEntityGeometryShader;
        Video::Shader* editorEntityFragmentShader;
        Video::ShaderProgram* editorEntityShaderProgram;
        
        DeferredLighting* deferredLighting;
        
        // Post-processing.
        PostProcessing* postProcessing;
        ColorFilter* colorFilter;
        FogFilter* fogFilter;
        FXAAFilter* fxaaFilter;
        GammaCorrectionFilter* gammaCorrectionFilter;
        GlowFilter* glowFilter;
        GlowBlurFilter* glowBlurFilter;
        
        // Editor entity textures.
        Texture2D* particleEmitterTexture;
        Texture2D* lightTexture;
        Texture2D* soundSourceTexture;
        Texture2D* cameraTexture;
        
        // Geometry.
        GLuint vertexBuffer;
        GLuint vertexArray;
};
