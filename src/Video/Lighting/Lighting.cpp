#include "Lighting.hpp"

#include <Utility/Log.hpp>

#include "../Geometry/Rectangle.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "Deferred.frag.hpp"
#include "FrameBuffer.hpp"
#include "ReadWriteTexture.hpp"
#include "RenderSurface.hpp"
#include "Buffer/StorageBuffer.hpp"

using namespace Video;

Lighting::Lighting(ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
    
    // Store light uniform locations.
    for (unsigned int lightIndex = 0; lightIndex < lightCount; ++lightIndex) {
        lightUniforms[lightIndex].position = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].position").c_str());
        lightUniforms[lightIndex].intensities = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].intensities").c_str());
        lightUniforms[lightIndex].attenuation = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].attenuation").c_str());
        lightUniforms[lightIndex].ambientCoefficient = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].ambientCoefficient").c_str());
        lightUniforms[lightIndex].coneAngle = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].coneAngle").c_str());
        lightUniforms[lightIndex].direction = shaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].direction").c_str());
    }

    lightBuffer = new StorageBuffer(sizeof(glm::vec4), GL_STATIC_DRAW); //GL_STATIC_DRAW

}

Lighting::~Lighting() {
    delete lightBuffer;
}

void Lighting::ClearLights() {
    lights.clear();
}

void Lighting::AddLight(const Light& light) {
    lights.push_back(light);
}

void Lighting::Render(const glm::mat4& inverseProjectionMatrix, RenderSurface* renderSurface) {
    //// Disable depth testing.
    //GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);
    // 
    //GLint oldDepthFunctionMode;
    //glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    //glDepthFunc(GL_ALWAYS);
    //
    //// Blending enabled for handling multiple light sources.
    //GLboolean blend = glIsEnabledi(GL_BLEND, 0);
    //glEnablei(GL_BLEND, 0);
    //glBlendEquationi(0, GL_FUNC_ADD);
    //glBlendFunci(0, GL_ONE, GL_ONE);
    //
    //shaderProgram->Use();
    //
    //
    //glBindVertexArray(rectangle->GetVertexArray());
    //
    //// Set uniforms.
    //glUniform1i(shaderProgram->GetUniformLocation("textureAlbedo"), 0);
    //glUniform1i(shaderProgram->GetUniformLocation("textureNormal"), 1);
    //glUniform1i(shaderProgram->GetUniformLocation("textureMetallic"), 2);
    //glUniform1i(shaderProgram->GetUniformLocation("textureRougness"), 3);
    //glUniform1i(shaderProgram->GetUniformLocation("tDepth"), 4);

    assert(lights.size() <= lightCount);

    //lightBuffer->Bind();
    //if (lights.size() > 0)
    //    lightBuffer->Write(lights.data(), 0, sizeof(Light) * lights.size());
    //lightBuffer->BindBase(2); // bind/unbind
    //lightBuffer->Unbind();

    lightBuffer->Bind();
    glm::vec4 data(1,1,1,1);
    lightBuffer->Write(&data, 0, sizeof(glm::vec4));
    lightBuffer->Unbind();
    lightBuffer->BindBase(5);
    //

    glUniform1i(shaderProgram->GetUniformLocation("lightCount"), lightCount);
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("inverseProjectionMatrix"), 1, GL_FALSE, &inverseProjectionMatrix[0][0]);
    
    // Render lights.
    unsigned int lightIndex = 0U;
    for (const Light& light : lights) {
        glUniform4fv(lightUniforms[lightIndex].position, 1, &light.position[0]);
        glUniform3fv(lightUniforms[lightIndex].intensities, 1, &light.intensities[0]);
        glUniform1f(lightUniforms[lightIndex].attenuation, light.attenuation);
        glUniform1f(lightUniforms[lightIndex].ambientCoefficient, light.ambientCoefficient);
        glUniform1f(lightUniforms[lightIndex].coneAngle, light.coneAngle);
        glUniform3fv(lightUniforms[lightIndex].direction, 1, &light.direction[0]);

        if (++lightIndex >= lightCount) {
            lightIndex = 0U;
            //glDrawElements(GL_TRIANGLES, rectangle->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        }
    }

    if (lightIndex != 0U) {
        glUniform1i(shaderProgram->GetUniformLocation("lightCount"), lightIndex);
        //glDrawElements(GL_TRIANGLES, rectangle->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}
