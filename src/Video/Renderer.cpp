#include "Renderer.hpp"

#include <GL/glew.h>

using namespace Video;

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
