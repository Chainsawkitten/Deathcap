#include "ShadowPass.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Video {
    ShadowPass::ShadowPass()
    {
    }

    void ShadowPass::InitDephtMap()
    {
        glGenFramebuffers(1, &depthMapFbo);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDHT,
            SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Check if perspective is corretc
        lightProjection = glm::perspective<float>(glm::radians(45.0f), 1.0f, 2.0f, 50.0f);

    }

    void ShadowPass::BindBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
            depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int ShadowPass::GetShadowWidth()
    {
        return SHADOW_WIDHT;
    }

    int ShadowPass::GetShadowHeight()
    {
        return SHADOW_HEIGHT;
    }

    int ShadowPass::GetDepthMapFbo()
    {
        return depthMapFbo;
    }
    VIDEO_API glm::mat4 ShadowPass::getLightProjection()
    {
        return lightProjection;
    }
}