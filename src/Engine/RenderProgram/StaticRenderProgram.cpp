#include "StaticRenderProgram.hpp"

#include <Video/Geometry/Geometry3D.hpp>
#include <Video/Shader/ShaderProgram.hpp>
#include "../Texture/Texture2D.hpp"
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include <Video/Culling/Frustum.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Video;

StaticRenderProgram::StaticRenderProgram(ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
}

StaticRenderProgram::~StaticRenderProgram() {
    this->shaderProgram = nullptr;
}

void StaticRenderProgram::PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    shaderProgram->Use();
    
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    viewProjectionMatrix = projectionMatrix * viewMatrix;
    
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);
}

void StaticRenderProgram::Render(Video::Geometry::Geometry3D* geometry, const Texture2D* diffuseTexture, const Texture2D* normalTexture, const Texture2D* specularTexture, const Texture2D* glowTexture, const glm::mat4 modelMatrix) const {
    Frustum frustum(viewProjectionMatrix * modelMatrix);
    if (frustum.Collide(geometry->GetAxisAlignedBoundingBox())) {
        glBindVertexArray(geometry->GetVertexArray());
        
        // Set texture locations
        glUniform1i(shaderProgram->GetUniformLocation("baseImage"), 0);
        glUniform1i(shaderProgram->GetUniformLocation("normalMap"), 1);
        glUniform1i(shaderProgram->GetUniformLocation("specularMap"), 2);
        glUniform1i(shaderProgram->GetUniformLocation("glowMap"), 3);
        
        // Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTexture->GetTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularTexture->GetTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, glowTexture->GetTextureID());
        
        // Render model.
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
        glUniformMatrix3fv(shaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMatrix)[0][0]);
        
        glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

void StaticRenderProgram::PostRender() const {
    glBindVertexArray(0);
}
