#include "RenderSurface.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    this->size = size;

    // Textures.
    albedoReadWriteTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGBA8);

    // Frame buffers.
    deferredFrameBuffer = new FrameBuffer({ albedoReadWriteTexture });
}

RenderSurface::~RenderSurface() {
    delete deferredFrameBuffer;

    delete albedoReadWriteTexture;
}

FrameBuffer* RenderSurface::GetDeferredFrameBuffer() const {
    return deferredFrameBuffer;
}

glm::vec2 RenderSurface::GetSize() const {
    return size;
}

