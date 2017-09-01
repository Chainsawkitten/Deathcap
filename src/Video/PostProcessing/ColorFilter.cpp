#include "ColorFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "PostColor.frag.hpp"

using namespace Video;

ColorFilter::ColorFilter(const glm::vec3& color) {
    Shader* vertexShader = new Shader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(POSTCOLOR_FRAG, POSTCOLOR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
    
    this->color = color;
    colorLocation = shaderProgram->GetUniformLocation("color");
}

ColorFilter::~ColorFilter() {
    delete shaderProgram;
}

ShaderProgram* ColorFilter::GetShaderProgram() const {
    return shaderProgram;
}

void ColorFilter::SetUniforms() {
    glUniform3fv(colorLocation, 1, &color[0]);
}

void ColorFilter::SetColor(const glm::vec3& color) {
    this->color = color;
}
