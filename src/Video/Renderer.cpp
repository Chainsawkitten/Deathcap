#include "Renderer.hpp"

#include <GL/glew.h>
#include "Lighting/Light.hpp"
#include "Lighting/Lighting.hpp"
#include "RenderProgram/StaticRenderProgram.hpp"
#include "RenderProgram/SkinRenderProgram.hpp"
#include "PostProcessing/PostProcessing.hpp"
#include "PostProcessing/GammaCorrectionFilter.hpp"
#include "RenderTarget.hpp"

using namespace Video;

Renderer::Renderer(const glm::vec2& screenSize) {
    this->screenSize = screenSize;
    lighting = new Lighting(screenSize);
    staticRenderProgram = new StaticRenderProgram();
    skinRenderProgram = new SkinRenderProgram();
    postProcessing = new PostProcessing(screenSize);
    gammaCorrectionFilter = new GammaCorrectionFilter();
}

Renderer::~Renderer() {
    delete lighting;
    delete staticRenderProgram;
    delete skinRenderProgram;
    delete postProcessing;
    delete gammaCorrectionFilter;
}

void Renderer::SetScreenSize(const glm::vec2& screenSize) {
    this->screenSize = screenSize;
    
    postProcessing->UpdateBufferSize(screenSize);
    delete lighting;
    lighting = new Lighting(screenSize);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::StartRendering() {
    lighting->SetTarget();
    lighting->ClearLights();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, static_cast<GLsizei>(screenSize.x), static_cast<GLsizei>(screenSize.y));
}

void Renderer::AddLight(const Video::Light& light) {
    lighting->AddLight(light);
}

void Renderer::Light(const glm::mat4& inverseProjectionMatrix) {
    postProcessing->GetRenderTarget()->SetTarget();
    lighting->Render(inverseProjectionMatrix);
}

void Renderer::PrepareStaticMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    staticRenderProgram->PreRender(viewMatrix, projectionMatrix);
}

void Renderer::RenderStaticMesh(Geometry::Geometry3D* geometry, const Texture* diffuseTexture, const Texture* normalTexture, const Texture* specularTexture, const Texture* glowTexture, const glm::mat4 modelMatrix) {
    staticRenderProgram->Render(geometry, diffuseTexture, normalTexture, specularTexture, glowTexture, modelMatrix);
}

void Renderer::PrepareSkinnedMeshRendering(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    skinRenderProgram->PreRender(viewMatrix, projectionMatrix);
}

void Renderer::RenderSkinnedMesh(const Video::Geometry::Geometry3D* geometry, const Video::Texture* diffuseTexture, const Video::Texture* normalTexture, const Video::Texture* specularTexture, const Video::Texture* glowTexture, const glm::mat4& modelMatrix, const std::vector<glm::mat4>& bones, const std::vector<glm::mat3>& bonesIT) {
    skinRenderProgram->Render(geometry, diffuseTexture, normalTexture, specularTexture, glowTexture, modelMatrix, bones, bonesIT);
}

void Renderer::GammaCorrect() {
    postProcessing->ApplyFilter(gammaCorrectionFilter);
}

void Renderer::DisplayResults(bool dither) {
    postProcessing->Render(dither);
}
