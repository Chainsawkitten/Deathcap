﻿#include "Video\ParticleSystemRenderer.hpp"

#include "ComputeParticleShader.comp.hpp"
#include "DefaultParticleShader.vert.hpp"
#include "DefaultParticleShader.geom.hpp"
#include "DefaultParticleShader.frag.hpp"
#include "Texture/Texture.hpp"

using namespace Video;

ParticleSystemRenderer::ParticleSystemRenderer()
{
    points = new ParticlePos[nr_particles];
    vels = new ParticleVelocity[nr_particles];
}

ParticleSystemRenderer::ParticleSystemRenderer(int count)
{
    points = new ParticlePos[count];
    vels = new ParticleVelocity[count];
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
}

void ParticleSystemRenderer::InitRender(const ParticlePos* particlesPos, const ParticleVelocity* particlesVelocity)
{

}

void ParticleSystemRenderer::Init()
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
}

void ParticleSystemRenderer::CreateStorageBuffers()
{

    for (int i = 0; i < this->nr_particles; i++)
    {
        points[i].x = 0.0f;
        points[i].y = 0.0f;
        points[i].z = 0.0f;
        points[i].w = 0.0f;
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
        vels[i].vx = 5.0f * (rand()%10) - 5;
        vels[i].vy = 5.0f * (rand() % 10) - 5;
        vels[i].vz = 0.0f;
        vels[i].life = 0.0f;
    }
    
    glGenBuffers(1, &velSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ParticlePos)*nr_particles, &vels[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

}

void ParticleSystemRenderer::Update(float dt, ParticleSystemRenderer::EmitterSettings settings)
{
    glm::vec3 InitVeloc = glm::vec3(5 * (rand() % 10) - 5, 5 * (rand() % 10) - 5, 0);
    timer += dt;

    computeShaderProgram->Use();

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, posSSbo, 0, nr_particles * sizeof(ParticlePos));
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, velSSbo, 0, nr_particles * sizeof(ParticlePos));
    
    glDispatchCompute(nr_particles/128, 1, 1);
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
            particleShootIndex.x = 0;
        }
        timer = 0.0f;
    }
}

void ParticleSystemRenderer::Draw(Texture* textureAtlas, unsigned int textureAtlasRows, const glm::mat4& viewProjectionMatrix, ParticleSystemRenderer::EmitterSettings settings)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    // Blending
    glEnablei(GL_BLEND, 0);
    glEnablei(GL_BLEND, 1);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE);
    glBlendFunci(1, GL_SRC_ALPHA, GL_ONE);
    shaderProgram->Use();
    glBindVertexArray(m_glDrawVAO);

    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);
    glUniform1i(shaderProgram->GetUniformLocation("baseImage"), 0);
    glUniform1f(shaderProgram->GetUniformLocation("textureIndex"), settings.textureIndex);

    // Base image texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas->GetTextureID());

    // Send the texture to shader.
    glUniform1f(shaderProgram->GetUniformLocation("textureAtlasRows"), textureAtlasRows);

    glDrawArrays(GL_POINTS, 0, nr_particles);

    glDisablei(GL_BLEND, 0);
    glDisablei(GL_BLEND, 1);

    //Cleanup
    //glDisableClientState(GL_VERTEX_ARRAY);
    glBindVertexArray(0);

}
