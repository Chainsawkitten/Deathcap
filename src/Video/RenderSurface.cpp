#include "RenderSurface.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    this->size = size;

    // Textures.
    albedoTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGB16);
    normalTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGB16);
    specTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGB8);
    glowTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGB8);
    depthTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::DEPTH32);

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

FrameBuffer* RenderSurface::GetDeferredFrameBuffer() const {
    return deferredFrameBuffer;
}

glm::vec2 RenderSurface::GetSize() const {
    return size;
}

