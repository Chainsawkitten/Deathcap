#pragma once

namespace Video {
    class Renderer;
    class RenderSurface;
    class Texture2D;
}
class World;
class Entity;

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
        
        void LightWorld(World& world, const Entity* camera);

        Video::Renderer* renderer;

        Video::RenderSurface* renderSurface;
        
        // Editor entity textures.
        Video::Texture2D* particleEmitterTexture;
        Video::Texture2D* lightTexture;
        Video::Texture2D* soundSourceTexture;
        Video::Texture2D* cameraTexture;
};
