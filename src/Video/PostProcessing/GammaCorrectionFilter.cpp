#include "GammaCorrectionFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "PostGamma.frag.hpp"

using namespace Video;

GammaCorrectionFilter::GammaCorrectionFilter() {
    Shader* vertexShader = new Shader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(POSTGAMMA_FRAG, POSTGAMMA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
    
    brightnessLocation = shaderProgram->GetUniformLocation("brightness");
}

GammaCorrectionFilter::~GammaCorrectionFilter() {
    delete shaderProgram;
}

ShaderProgram* GammaCorrectionFilter::GetShaderProgram() const {
    return shaderProgram;
}

void GammaCorrectionFilter::SetUniforms() {
    glUniform1f(brightnessLocation, 1.f / brightness);
}

void GammaCorrectionFilter::SetBrightness(float brightness) {
    this->brightness = brightness;
}
