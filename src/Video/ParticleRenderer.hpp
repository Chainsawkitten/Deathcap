#pragma once

#include <glm/glm.hpp>

namespace Video {
    /// Renders particles.
    class ParticleRenderer {
        public:
            /// A particle in the particle system.
            struct Particle {
                /// Position.
                glm::vec3 worldPos;
                
                /// Size.
                glm::vec2 size;
                
                /// Life (in seconds).
                float life;
                
                /// Lifetime (in seconds).
                float lifetime;
                
                /// Initial velocity.
                glm::vec3 velocity;
                
                /// Start, mid and end of life alpha of particle.
                glm::vec3 alpha;
                
                /// Color of the particle.
                glm::vec3 color;
                
                /// Texture index (for the texture atlas, left to right, top to bottom indexing)
                float textureIndex;
            };
            
            /// Create new particle renderer.
            ParticleRenderer();
            
            /// Destructor.
            ~ParticleRenderer();
    };
}
