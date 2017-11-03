#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <random>
#include <Video/ParticleSystemRenderer.hpp>
#include "../Entity/ComponentContainer.hpp"
#include "../linking.hpp"

class Entity;
class World;
namespace Video {
    class Texture2D;
    class ParticleRenderer;
    class ParticleSystemRenderer;
}
namespace Component {
    class ParticleEmitter;
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
        
        /// Update particle buffer.
        /**
         * Needs to be called before rendering (but only once a frame).
         * @param world The world to render.
         */
        ENGINE_API void UpdateBuffer(World& world);

        /// Render the particles in a world.
        /**
         * @param world %World containing particles to render.
         * @param position World position of the camera.
         * @param up Up direction of the camera.
         * @param viewProjectionMatrix View projection matrix of the camera.
         */
        ENGINE_API void Render(World& world, const glm::vec3& position, const glm::vec3& up, const glm::mat4& viewProjectionMatrix);


        /// Update particle buffer.
        /**
        * Needs to be called before rendering (but only once a frame).
        * @param world The world to render.
        */
        ENGINE_API void UpdateParticleSystem(World& world, Component::ParticleSystemComponent* particleSystem);

        /**
        * @param world %World containing number of particle systems to render.
        */
        ENGINE_API void RenderParticleSystem(World& world, const glm::mat4& viewProjectionMatrix);
        
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
         * @return The created component.
         */
        ENGINE_API Component::ParticleEmitter* CreateParticleEmitter();

        /// Create particle emitter component.
        /**
        * @param World to create particle system in.
        * @return The created component.
        */
        ENGINE_API Component::ParticleSystemComponent* CreateAParticleSystem(World * world);
        
        /// Create particle emitter component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        ENGINE_API Component::ParticleEmitter* CreateParticleEmitter(const Json::Value& node);

        /// Create particle System component.
        /**
        * @param node Json node to load the component from.
        * @return The created component.
        */
        ENGINE_API Component::ParticleSystemComponent* CreateParticleSystem(const Json::Value& node);
        
        /// Get all particle emitter components.
        /**
         * @return All particle emitter components.
         */
        ENGINE_API const std::vector<Component::ParticleEmitter*>& GetParticleEmitters() const;
        
        /// Remove all killed components.
        ENGINE_API void ClearKilledComponents();
        
    private:
        ParticleManager();
        ~ParticleManager();
        ParticleManager(ParticleManager const&) = delete;
        void operator=(ParticleManager const&) = delete;
        
        // Decide where the emitter should emit before rendering.
        void EmitParticle(World& world, Component::ParticleEmitter* emitter);
        
        // Emit a particle at the given position.
        void EmitParticle(World& world, const glm::vec3& position, Component::ParticleEmitter* emitter);
        
        unsigned int maxParticleCount = 10000;
        
        std::random_device randomDevice;
        std::mt19937 randomEngine;
        
        Video::ParticleRenderer* particleRenderer;
        std::vector<Video::ParticleSystemRenderer> particleSystemRenderers;

        std::vector<Video::ParticleSystemRenderer::EmitterSettings> emitterSettings;

        // The number of rows in the texture atlas.
        int textureAtlasRowNumber = 4;

        // Texture atlas containing the particle textures.
        Video::Texture2D* textureAtlas;
        
        ComponentContainer<Component::ParticleEmitter> particleEmitters;
        ComponentContainer<Component::ParticleSystemComponent> particleSystems;
};
