#include "RenderSurface.hpp"

#include "FrameBuffer.hpp"
#include "ReadWriteTexture.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    this->size = size;

    // Deferred textures.
    albedoTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB16F, GL_FLOAT);
    normalTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB16F, GL_FLOAT);
    specTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB, GL_FLOAT);
    glowTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB, GL_FLOAT);
    depthTexture = new ReadWriteTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT);

    // Post processing textures.
    colorTexture[0] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA16F, GL_UNSIGNED_BYTE);
    colorTexture[1] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA16F, GL_UNSIGNED_BYTE);
    extraTexture[0] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    extraTexture[1] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    depth2Texture[0] = new ReadWriteTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT);
    depth2Texture[1] = new ReadWriteTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT);

    // Frame buffers.
    deferredFrameBuffer = new FrameBuffer({ albedoTexture, normalTexture, specTexture, glowTexture, depthTexture });
    postProcessingFrameBuffer[0] = new FrameBuffer({ colorTexture[0], extraTexture[0], depth2Texture[0] });
    postProcessingFrameBuffer[1] = new FrameBuffer({ colorTexture[1], extraTexture[1], depth2Texture[1] });
}

RenderSurface::~RenderSurface() {
    delete deferredFrameBuffer;
    delete postProcessingFrameBuffer[0];
    delete postProcessingFrameBuffer[1];

    delete albedoTexture;
    delete normalTexture;
    delete specTexture;
    delete glowTexture;
    delete depthTexture;

    delete colorTexture[0];
    delete colorTexture[1];
    delete extraTexture[0];
    delete extraTexture[1];
    delete depth2Texture[0];
    delete depth2Texture[1];
}

glm::vec2 RenderSurface::GetSize() const {
    return size;
}


FrameBuffer* RenderSurface::GetDeferredFrameBuffer() const {
    return deferredFrameBuffer;
}

ReadWriteTexture* RenderSurface::GetAlbedoTexture() const {
    return albedoTexture;
}

ReadWriteTexture* RenderSurface::GetNormalTexture() const {
    return normalTexture;
}

ReadWriteTexture* RenderSurface::GetSpecularTexture() const {
    return specTexture;
}

ReadWriteTexture* RenderSurface::GetGlowTexture() const {
    return glowTexture;
}

ReadWriteTexture* RenderSurface::GetDepthTexture() const {
    return depthTexture;
}
