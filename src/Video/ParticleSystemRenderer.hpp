#pragma once
#include <glm\glm.hpp>
#include "Shader\Shader.hpp"
#include "Shader\ShaderProgram.hpp"

#include "linking.hpp"

struct ParticlePos
{
    float x;
    float y;
    float z;
    float w;          //Holds the speed so when we get this in the pixel shader we can color our particles based on that.
};


//ParticleVelocity are only used in the compute shader.
//This is why I keep the  fTimeToLive here.
struct ParticleVelocity
{
    float vx;
    float vy;
    float vz;
    float life; //Remaining time. 
};

struct ParticleColor {
    float cx;
    float cy;
    float cz;
    float ca; //Remaining time. 
};

namespace Video
{
    class Texture;

    class ParticleSystemRenderer
    {
    public:
        struct EmitterSettings {
            /// Position.
            glm::vec3 worldPos;

            /// Size.
            glm::vec2 size;

            /// Life (in seconds).
            float life;

            /// Lifetime (in seconds).
            float lifetime = 10;

            /// Initial velocity.
            glm::vec3 velocity;

            /// Start, mid and end of life alpha of particle.
            glm::vec3 alpha;

            /// Color of the particle.
            glm::vec3 color;

            /// Fire rate.
            float rate = 1.0f;

            /// Texture index (for the texture atlas, left to right, top to bottom indexing)
            int textureIndex = 0;

            /// Multiply velocity.
            float velocityMultiplier = 10.0f;

            /// Gives spread to the particles
            int spread = 1;

            /// Used to give random velocity to particles at all time.
            glm::vec3 randomVec = glm::vec3(1, 1, 1);

            /// Scale.
            float scale = 1.0f;

            /// Particle mass.
            float alpha_control = 1.0f;

        };

        EmitterSettings emitterSettings;

        VIDEO_API ParticleSystemRenderer();
        VIDEO_API ParticleSystemRenderer(int count);
        VIDEO_API ~ParticleSystemRenderer();

        VIDEO_API void Init();

        VIDEO_API void CreateStorageBuffers();

        //Particles are sent to the compute shader and we compute there the new positions/velocities.
        VIDEO_API void Update(float dt, EmitterSettings settings);

        //Render the particles.
        VIDEO_API void Draw(Texture* textureAtlas, unsigned int textureAtlasRows , const glm::mat4& viewProjectionMatrix, ParticleSystemRenderer::EmitterSettings settings);

    private:
        VIDEO_API void InitRender(const ParticlePos* particlesPos, const ParticleVelocity* particlesVelocity);


    private:

        Video::ShaderProgram* computeShaderProgram;

        Video::ShaderProgram* shaderProgram;

        int WORK_GROUP_SIZE = 128;

        unsigned int        nr_particles = 1024;
        glm::vec2		    particleShootIndex = glm::vec2(0, 10);
        int                 nr_new_particles = 32;
        float               rate = 1000.0f;
        float               delta_time = 0.0f;
        float               speed = 1.0f;
        float               variation = 2.0f;
        float		        lifeLength = 10.0f;
        bool                fade_alpha = true;
        glm::vec4	        basecolor;
        glm::vec3	        meshScale;
        glm::vec3           velocity;
        ParticlePos *points;
        ParticleVelocity *vels;
        ParticleColor *col;

        float timer = 0.0f;

        GLuint posSSbo;
        GLuint velSSbo;
        GLuint colSSbo;
        GLuint m_glDrawVAO;

    };
}

