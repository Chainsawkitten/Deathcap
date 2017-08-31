#pragma once

class World;
class Entity;
namespace Video {
    class Renderer;
}

/// Holds the frame buffers used for deferred rendering.
class DeferredLighting {
    public:
        /// Create new render target.
        /**
         * @param renderer Renderer to use for rendering.
         */
        DeferredLighting(Video::Renderer* renderer);
        
        /// Render the lighting in the world.
        /**
         * @param world World to get the lights from.
         * @param camera Camera to use.
         */
        void Render(World& world, const Entity* camera);
        
    private:
        Video::Renderer* renderer;
};
