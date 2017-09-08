#include "FogFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "PostFog.frag.hpp"

using namespace Video;

FogFilter::FogFilter(const glm::vec3& color) {
    Shader* vertexShader = new Shader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(POSTFOG_FRAG, POSTFOG_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
    
    this->color = color;
    colorLocation = shaderProgram->GetUniformLocation("color");
    
    matrixLocation = shaderProgram->GetUniformLocation("inverseProjectionMatrix");
    
    density = 0.01f;
    densityLocation = shaderProgram->GetUniformLocation("density");
}

FogFilter::~FogFilter() {
    delete shaderProgram;
}

void FogFilter::SetColor(const glm::vec3& color) {
    this->color = color;
}

void FogFilter::SetDensity(float density) {
    this->density = density;
}

void FogFilter::SetProjectionMatrix(const glm::mat4& projectionMatrix) {
    this->projectionMatrix = projectionMatrix;
}

ShaderProgram* FogFilter::GetShaderProgram() const {
    return shaderProgram;
}

void FogFilter::SetUniforms() {
    glUniform3fv(colorLocation, 1, &color[0]);
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &glm::inverse(projectionMatrix)[0][0]);
    glUniform1f(densityLocation, density);
}
