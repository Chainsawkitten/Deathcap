#include "RenderSurface.hpp"

using namespace Video;

RenderSurface::RenderSurface(const glm::vec2& size) {
    albedoReadWriteTexture = new ReadWriteTexture(size, ReadWriteTexture::FORMAT::RGBA8);
    deferredFrameBuffer = new FrameBuffer({ albedoReadWriteTexture });
}

RenderSurface::~RenderSurface() {
    delete deferredFrameBuffer;
    delete albedoReadWriteTexture;
}
