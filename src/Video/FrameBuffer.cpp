#include "FrameBuffer.hpp"

#include <Utility/Log.hpp>

using namespace Video;

FrameBuffer::FrameBuffer(const std::vector<ReadWriteTexture*>& readWriteTextures) {

    // Frame buffer object.
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

    std::vector<GLenum> drawBuffers(readWriteTextures.size());
    for (std::size_t i = 0; i < readWriteTextures.size(); ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, readWriteTextures[i]->GetTexture(), 0);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Framebuffer creation failed\n";
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &frameBufferObject);
}

void FrameBuffer::SetTarget() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferObject);
}
