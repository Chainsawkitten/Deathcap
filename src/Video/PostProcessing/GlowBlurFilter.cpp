#include "GlowBlurFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "PostGlowBlur.frag.hpp"

using namespace Video;

GlowBlurFilter::GlowBlurFilter() {
    Shader* vertexShader = new Shader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(POSTGLOWBLUR_FRAG, POSTGLOWBLUR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
    
    screenSizeLocation = shaderProgram->GetUniformLocation("screenSize");
    
    horizontal = false;
    horizontalLocation = shaderProgram->GetUniformLocation("horizontal");
}

GlowBlurFilter::~GlowBlurFilter() {
    delete shaderProgram;
}

ShaderProgram* GlowBlurFilter::GetShaderProgram() const {
    return shaderProgram;
}

void GlowBlurFilter::SetUniforms() {
    glUniform2fv(screenSizeLocation, 1, &screenSize[0]);
    glUniform1f(horizontalLocation, horizontal);
}

void GlowBlurFilter::SetScreenSize(const glm::vec2& screenSize) {
    this->screenSize = screenSize;
}

void GlowBlurFilter::SetHorizontal(bool horizontal) {
    this->horizontal = horizontal ? 1.f : 0.f;
}
