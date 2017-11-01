#include "Video\ParticleSystem.hpp"

#include "ComputeParticleShader.comp.hpp"
#include "DefaultParticleShader.vert.hpp"
#include "DefaultParticleShader.geom.hpp"
#include "DefaultParticleShader.frag.hpp"

GLuint shootIndex;
GLuint rateUni;
GLuint timerUni;
GLuint velocityUni;

using namespace Particles;

ParticleSystem::ParticleSystem()
{
    points = new ParticlePos[nr_particles];
    vels = new ParticleVelocity[nr_particles];
}

ParticleSystem::ParticleSystem(int count)
{
    points = new ParticlePos[count];
    vels = new ParticleVelocity[count];
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::InitRender(const ParticlePos* particlesPos, const ParticleVelocity* particlesVelocity)
{

}

void ParticleSystem::Init()
{
    // Load shaders.
    Video::Shader* vertexShader =   new Video::Shader(DEFAULTPARTICLESHADER_VERT, DEFAULTPARTICLESHADER_VERT_LENGTH, GL_VERTEX_SHADER);
    Video::Shader* geometryShader = new Video::Shader(DEFAULTPARTICLESHADER_GEOM, DEFAULTPARTICLESHADER_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    Video::Shader* fragmentShader = new Video::Shader(DEFAULTPARTICLESHADER_FRAG, DEFAULTPARTICLESHADER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new Video::ShaderProgram({ vertexShader, geometryShader, fragmentShader });
    delete vertexShader;
    delete geometryShader;
    delete fragmentShader;

    Video::Shader* computeShader = new Video::Shader(COMPUTEPARTICLESHADER_COMP, COMPUTEPARTICLESHADER_COMP_LENGTH, GL_COMPUTE_SHADER);
    computeShaderProgram = new Video::ShaderProgram({computeShader});
    delete computeShader;

    shootIndex = computeShaderProgram->GetUniformLocation("ShootIndex");
    timerUni = computeShaderProgram->GetUniformLocation("timer");
    rateUni = computeShaderProgram->GetUniformLocation("rate");
    velocityUni = computeShaderProgram->GetUniformLocation("InitVel");
}

void ParticleSystem::CreateStorageBuffers()
{

    for (int i = 0; i < this->nr_particles; i++)
    {
        points[i].x = 0.0f;
        points[i].y = 0.0f;
        points[i].z = 0.0f;
        points[i].w = 1.0f;
    }

    glGenBuffers(1, &posSSbo);
    glBindBuffer(GL_ARRAY_BUFFER, posSSbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticlePos) * nr_particles, &points[0], GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ParticlePos)*nr_particles, &points[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenVertexArrays(1, &m_glDrawVAO);
    glBindVertexArray(m_glDrawVAO);
    glBindBuffer(GL_ARRAY_BUFFER, posSSbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticlePos) * nr_particles, &points[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticlePos), 0);

    glBindVertexArray(0);

    for (int i = 0; i < this->nr_particles; i++)
    {
        vels[i].vx = 1.0f;
        vels[i].vy = 1.0f;
        vels[i].vz = 0.0f;
        vels[i].fTimeToLive = 1.0f;
    }
    
    glGenBuffers(1, &velSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ParticlePos)*nr_particles, &vels[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

}

void ParticleSystem::Update(float dt)
{
    glm::vec3 InitVeloc = glm::vec3(1, 1, 0);
    timer += dt;
    computeShaderProgram->Use();
    glUniform2fv(shootIndex, 1, &this->particleShootIndex[0]);
    glUniform1f(rateUni, rate);
    glUniform1f(timerUni, timer);
    glUniform3fv(velocityUni, 1, &InitVeloc[0]);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, posSSbo, 0, nr_particles * sizeof(ParticlePos));
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, velSSbo, 0, nr_particles * sizeof(ParticlePos));
    
    glDispatchCompute((nr_particles/2)/32, (nr_particles / 2)/32, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glUseProgram(0);
    if (timer >= rate)
    {
        particleShootIndex.x++;
        particleShootIndex.y++;
        if (particleShootIndex.y > nr_particles)
        {
            particleShootIndex.y = 1;
            particleShootIndex.x = 1;
        }
        timer = 0.0f;
    }

}

void ParticleSystem::Draw(GLuint programID, const glm::mat4& viewProjectionMatrix)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    shaderProgram->Use();

    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);

    glBindVertexArray(m_glDrawVAO);
    glDrawArrays(GL_POINTS, 0, nr_particles);

    //Cleanup
    //glDisableClientState(GL_VERTEX_ARRAY);
    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}
