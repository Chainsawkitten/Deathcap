#include "Renderer.hpp"

#include <GL/glew.h>
#include "Lighting/Light.hpp"
#include "Lighting/Lighting.hpp"

using namespace Video;

Renderer::Renderer(const glm::vec2& screenSize) {
    this->screenSize = screenSize;
    lighting = new Lighting(screenSize);
}

Renderer::~Renderer() {
    delete lighting;
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

void Renderer::AddLight(const Light& light) {
    lighting->AddLight(light);
}
