#include "RenderSurface.hpp"

#include "FrameBuffer.hpp"
#include "ReadWriteTexture.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    this->size = size;

    // Textures.
    albedoTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB16F, GL_FLOAT);
    normalTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB16F, GL_FLOAT);
    specTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB, GL_FLOAT);
    glowTexture = new ReadWriteTexture(size, GL_RGB, GL_RGB, GL_FLOAT);
    depthTexture = new ReadWriteTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT);

    // Frame buffers.
    deferredFrameBuffer = new FrameBuffer({ albedoTexture, normalTexture, specTexture, glowTexture, depthTexture });
}

RenderSurface::~RenderSurface() {
    delete deferredFrameBuffer;

    delete albedoTexture;
    delete normalTexture;
    delete specTexture;
    delete glowTexture;
    delete depthTexture;
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
