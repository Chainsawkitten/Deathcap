#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <random>
#include <map>
#include <Video/ParticleSystemRenderer.hpp>
#include "../Entity/ComponentContainer.hpp"
#include "../linking.hpp"

class World;
namespace Video {
    class Texture2D;
    class ParticleSystemRenderer;
    class RenderSurface;
}
namespace Component {
    class ParticleSystemComponent;
}
namespace Json {
    class Value;
}

/// Handles particles.
class ParticleManager {
    friend class Hub;
    
    public:
        /// Get the maximum amount of particles.
        /**
         * @return Maximum amount of particles.
         */
        ENGINE_API unsigned int GetMaxParticleCount() const;
        
        /// Update all the system's particles, spawn new particles etc.
        /**
         * @param world World to update.
         * @param time Time since last frame (in seconds).
         * @param preview Whether to only update particle emitters that are being previewed.
         */
        ENGINE_API void Update(World& world, float time, bool preview = false);

        /// Render the particles in a world.
        /**
         * @param world %World containing particles to render.
         * @param position World position of the camera.
         * @param up Up direction of the camera.
         * @param viewProjectionMatrix View projection matrix of the camera.
         * @param renderSurface %RenderSurface to render particles to.
         */

        /// Renders particlesystem.
        /**
         * @param viewProjectionMatrix The view-projection matrix from the camera.
         */
        ENGINE_API void RenderParticleSystem(const glm::mat4& viewProjectionMatrix);
        
        /// Get the texture atlas.
        /**
         * @return The particle texture atlas.
         */
        ENGINE_API const Video::Texture2D* GetTextureAtlas() const;
        
        /// Get the number of rows in the texture atlas.
        /**
         * @return The number of rows in the texture atlas.
         */
        ENGINE_API int GetTextureAtlasRows() const;

        /// Create particle emitter component.
        /**
         * @param World to create particle system in.
         * @return The created component.
         */
        ENGINE_API Component::ParticleSystemComponent* CreateAParticleSystem(World * world);

        /// Create particle System component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        ENGINE_API Component::ParticleSystemComponent* CreateParticleSystem(const Json::Value& node);

        /// Get all particle emitter components.
        /**
         * @param component Component to remove.
         */
        ENGINE_API void RemoveParticleRenderer(Component::ParticleSystemComponent* component);
        
        /// Remove all killed components.
        ENGINE_API void ClearKilledComponents();
        
    private:
        ParticleManager();
        ~ParticleManager();
        ParticleManager(ParticleManager const&) = delete;
        void operator=(ParticleManager const&) = delete;

        // Inits the particle emitter.
        Component::ParticleSystemComponent* InitParticleSystem(Component::ParticleSystemComponent* component);
        
        unsigned int maxParticleCount = 10000;
        
        std::random_device randomDevice;
        std::mt19937 randomEngine;

        std::map<Component::ParticleSystemComponent*, Video::ParticleSystemRenderer*> particleSystemRenderers;

        std::map<Component::ParticleSystemComponent*, Video::ParticleSystemRenderer::EmitterSettings> emitterSettings;

        // The number of rows in the texture atlas.
        int textureAtlasRowNumber = 6;

        // Texture atlas containing the particle textures.
        Video::Texture2D* textureAtlas;
        
        ComponentContainer<Component::ParticleSystemComponent> particleSystems;
};
