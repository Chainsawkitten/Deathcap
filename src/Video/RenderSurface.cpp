#include "RenderSurface.hpp"

#include "Buffer/FrameBuffer.hpp"
#include "Buffer/ReadWriteTexture.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    this->size = size;

    // Textures.
    depthTexture = new ReadWriteTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT);
    colorTexture[0] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
    colorTexture[1] = new ReadWriteTexture(size, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);

    // Frame buffers.
    depthFrameBuffer = new FrameBuffer({ depthTexture });
    colorFrameBuffer[0] = new FrameBuffer({ colorTexture[0] });
    colorFrameBuffer[1] = new FrameBuffer({ colorTexture[1] });
}

RenderSurface::~RenderSurface() {
    delete depthFrameBuffer;
    delete colorFrameBuffer[0];
    delete colorFrameBuffer[1];

    delete depthTexture;
    delete colorTexture[0];
    delete colorTexture[1];
}

glm::vec2 RenderSurface::GetSize() const {
    return size;
}

FrameBuffer* RenderSurface::GetDepthFrameBuffer() const {
    return depthFrameBuffer;
}

FrameBuffer* RenderSurface::GetColorFrameBuffer() const {
    return colorFrameBuffer[1 - which];
}

ReadWriteTexture* RenderSurface::GetDepthTexture() const {
    return depthTexture;
}

ReadWriteTexture* RenderSurface::GetColorTexture() const {
    return colorTexture[which];
}

void RenderSurface::Swap() {
    which = 1 - which;
}
