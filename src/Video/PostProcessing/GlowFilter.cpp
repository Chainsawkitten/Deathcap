#include "GlowFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "PostGlow.frag.hpp"

using namespace Video;

GlowFilter::GlowFilter() {
    Shader* vertexShader = new Shader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(POSTGLOW_FRAG, POSTGLOW_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram({ vertexShader, fragmentShader });
    delete vertexShader;
    delete fragmentShader;
}

GlowFilter::~GlowFilter() {
    delete shaderProgram;
}

ShaderProgram* GlowFilter::GetShaderProgram() const {
    return shaderProgram;
}

void GlowFilter::SetUniforms() {
    
}
