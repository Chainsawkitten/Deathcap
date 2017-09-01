#include "SkinRenderProgram.hpp"

#include "../Component/Lens.hpp"
#include "../Component/Material.hpp"
#include "../Component/Mesh.hpp"
#include "../Entity/Entity.hpp"
#include "../Geometry/RiggedModel.hpp"
#include <Video/Shader/ShaderProgram.hpp>
#include "../Texture/Texture2D.hpp"
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include <Video/Culling/Frustum.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

SkinRenderProgram::SkinRenderProgram(Video::ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
}

SkinRenderProgram::~SkinRenderProgram() {
    this->shaderProgram = nullptr;
}

void SkinRenderProgram::PreRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    shaderProgram->Use();
    
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    viewProjectionMatrix = projectionMatrix * viewMatrix;
    
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMatrix[0][0]);
}

void SkinRenderProgram::Render(Mesh* mesh) const {
    Entity* entity = mesh->entity;
    Material* material = entity->GetComponent<Material>();
    if (mesh->geometry == nullptr || material == nullptr)
        return;
    
    glm::mat4 modelMatrix = entity->GetModelMatrix();
    
    Video::Frustum frustum(viewProjectionMatrix * modelMatrix);
    if (frustum.Collide(mesh->geometry->GetAxisAlignedBoundingBox())) {
        Geometry::RiggedModel* model = static_cast<Geometry::RiggedModel*>(mesh->geometry);
        
        glBindVertexArray(mesh->geometry->GetVertexArray());
        
        // Set texture locations
        glUniform1i(shaderProgram->GetUniformLocation("baseImage"), 0);
        glUniform1i(shaderProgram->GetUniformLocation("normalMap"), 1);
        glUniform1i(shaderProgram->GetUniformLocation("specularMap"), 2);
        glUniform1i(shaderProgram->GetUniformLocation("glowMap"), 3);
        
        // Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->diffuse->GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->normal->GetTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->specular->GetTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, material->glow->GetTextureID());
        
        // Render model.
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
        glUniformMatrix3fv(shaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMatrix)[0][0]);
        const std::vector<glm::mat4>& bones = model->skeleton.GetFinalTransformations();
        const std::vector<glm::mat3>& bonesIT = model->skeleton.GetFinalTransformationsIT();
        assert(bones.size() <= 100 && bonesIT.size() <= 100);
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("bones"), bones.size(), GL_FALSE, &bones[0][0][0]);
        glUniformMatrix3fv(shaderProgram->GetUniformLocation("bonesIT"), bonesIT.size(), GL_FALSE, &bonesIT[0][0][0]);
        
        glDrawElements(GL_TRIANGLES, mesh->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

void SkinRenderProgram::PostRender() const {
    glBindVertexArray(0);
}
