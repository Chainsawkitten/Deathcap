#include "SkinRenderProgram.hpp"

#include "../Geometry/Geometry3D.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Culling/Frustum.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Buffer/StorageBuffer.hpp"
#include "Skinning.vert.hpp"
#include "Default3D.frag.hpp"
#include "ZrejectionSkin.vert.hpp"
#include "Zrejection.frag.hpp"
#include "ShadowSkin.vert.hpp"
#include <chrono>
#include <Utility/Log.hpp>

using namespace Video;

SkinRenderProgram::SkinRenderProgram() {
    Shader* vertexShader = new Shader(SKINNING_VERT, SKINNING_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader} );
    delete vertexShader;
    delete fragmentShader;

    // Create shaders for early rejection pass
    vertexShader = new Shader(ZREJECTIONSKIN_VERT, ZREJECTIONSKIN_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = new Shader(ZREJECTION_FRAG, ZREJECTION_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    zShaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;

    // Create shaders for shadowpass
    vertexShader = new Shader(SHADOWSKIN_VERT, SHADOWSKIN_VERT_LENGTH, GL_VERTEX_SHADER);
    shadowProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
}

SkinRenderProgram::~SkinRenderProgram() {
    delete shaderProgram;
    delete zShaderProgram;
    delete shadowProgram;
}

void SkinRenderProgram::PreShadowRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int shadowId, int shadowWidth, int shadowHeight, int depthFbo) {
    // Cull front faces to avoid peter panning.
    glCullFace(GL_FRONT);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    this->shadowProgram->Use();

    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->lightSpaceMatrix = projectionMatrix * viewMatrix;
    this->viewProjectionMatrix = projectionMatrix * viewMatrix;
    this->shadowId = shadowId;

    glUniformMatrix4fv(shadowProgram->GetUniformLocation("lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

void SkinRenderProgram::ShadowRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones) const {
    Frustum frustum(viewProjectionMatrix * modelMatrix);
    if (frustum.Collide(geometry->GetAxisAlignedBoundingBox())) {
        glBindVertexArray(geometry->GetVertexArray());

        glUniformMatrix4fv(shadowProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMatrix[0][0]);
        assert(bones.size() <= 50);
        glUniformMatrix4fv(shadowProgram->GetUniformLocation("bones"), bones.size(), GL_FALSE, &bones[0][0][0]);

        glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}


void SkinRenderProgram::PreDepthRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    this->zShaderProgram->Use();

    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->viewProjectionMatrix = projectionMatrix * viewMatrix;

    glUniformMatrix4fv(zShaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);
}

void SkinRenderProgram::DepthRender(Geometry::Geometry3D* geometry, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones) const {
    Frustum frustum(viewProjectionMatrix * modelMatrix);
    if (frustum.Collide(geometry->GetAxisAlignedBoundingBox())) {
        glBindVertexArray(geometry->GetVertexArray());

        glUniformMatrix4fv(zShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMatrix[0][0]);
        assert(bones.size() <= 50);
        glUniformMatrix4fv(zShaderProgram->GetUniformLocation("bones"), bones.size(), GL_FALSE, &bones[0][0][0]);

        glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

void SkinRenderProgram::PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const StorageBuffer* lightBuffer, unsigned int lightCount) {
    shaderProgram->Use();
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->viewProjectionMatrix = projectionMatrix * viewMatrix;

    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);

    // Lights.
    glUniform1i(shaderProgram->GetUniformLocation("lightCount"), lightCount);
    lightBuffer->BindBase(5);

    // Post processing.
    {
        float gamma = 2.2f;
        glUniform1fv(shaderProgram->GetUniformLocation("gamma"), 1, &gamma);
    }

    {
        int fogApply = false;
        float fogDensity = 0.002f;
        glm::vec3 fogColor = glm::vec3(1, 0, 0);
        glUniform1iv(shaderProgram->GetUniformLocation("fogApply"), 1, &fogApply);
        glUniform1fv(shaderProgram->GetUniformLocation("fogDensity"), 1, &fogDensity);
        glUniform3fv(shaderProgram->GetUniformLocation("fogColor"), 1, &fogColor[0]);
    }

    {
        int colorFilterApply = false;
        glm::vec3 colorFilterColor = glm::vec3(0, 1, 0);
        glUniform1iv(shaderProgram->GetUniformLocation("colorFilterApply"), 1, &colorFilterApply);
        glUniform3fv(shaderProgram->GetUniformLocation("colorFilterColor"), 1, &colorFilterColor[0]);
    }

    {
        int ditherApply = true;
        float time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1000000000.0;
        glUniform1iv(shaderProgram->GetUniformLocation("ditherApply"), 1, &ditherApply);
        glUniform1fv(shaderProgram->GetUniformLocation("time"), 1, &time);
    }
}

void SkinRenderProgram::Render(const Geometry::Geometry3D* geometry, const Texture2D* textureAlbedo, const Texture2D* textureNormal, const Texture2D* textureMetallic, const Texture2D* textureRoughness, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones, bool isSelected) const {
    Frustum frustum(viewProjectionMatrix * modelMatrix);
    if (frustum.Collide(geometry->GetAxisAlignedBoundingBox())) {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        glBindVertexArray(geometry->GetVertexArray());
        
        // Set texture locations.
        glUniform1i(shaderProgram->GetUniformLocation("isSelected"), false);
        glUniform1i(shaderProgram->GetUniformLocation("mapAlbedo"), 0);
        glUniform1i(shaderProgram->GetUniformLocation("mapNormal"), 1);
        glUniform1i(shaderProgram->GetUniformLocation("mapMetallic"), 2);
        glUniform1i(shaderProgram->GetUniformLocation("mapRoughness"), 3);
        
        // Textures.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAlbedo->GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureNormal->GetTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureMetallic->GetTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textureRoughness->GetTextureID());
        
        // Render model.
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
        glUniformMatrix3fv(shaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMatrix)[0][0]);
        assert(bones.size() <= 50);
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("bones"), bones.size(), GL_FALSE, &bones[0][0][0]);
        
        glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);

        if (isSelected) {
            glUniform1i(shaderProgram->GetUniformLocation("isSelected"), true);
            glLineWidth(2.0f);
            for (int i = 0; i < geometry->GetIndexCount(); i += 3)
                glDrawArrays(GL_LINE_LOOP, i, 3);
        }

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
}
