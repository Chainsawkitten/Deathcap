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
    float fTimeToLive; //Remaining time. 
};

namespace Particles
{
    class ParticleSystem
    {
    public:
        VIDEO_API ParticleSystem();
        VIDEO_API ParticleSystem(int count);
        VIDEO_API ~ParticleSystem();

        VIDEO_API void Init();

        VIDEO_API void CreateStorageBuffers();

        //Particles are sent to the compute shader and we compute there the new positions/velocities.
        VIDEO_API void Update(float dt);

        //Render the particles.
        VIDEO_API void Draw(GLuint programID, const glm::mat4& viewProjectionMatrix);

    private:
        VIDEO_API void InitRender(const ParticlePos* particlesPos, const ParticleVelocity* particlesVelocity);


    private:

        Video::ShaderProgram* computeShaderProgram;

        Video::ShaderProgram* shaderProgram;

        int WORK_GROUP_SIZE = 128;

        unsigned int        nr_particles = 1024;
        glm::vec2		    particleShootIndex = glm::vec2(0, 1);
        int                 nr_new_particles = 1;
        float               rate = 10.0f;
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

        float timer = 0.0f;

        GLuint posSSbo;
        GLuint velSSbo;
        GLuint m_glDrawVAO;

    };
}

