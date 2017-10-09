#include "FrameBuffer.hpp"

#include <Utility/Log.hpp>
#include "ReadWriteTexture.hpp"

using namespace Video;

FrameBuffer::FrameBuffer(const std::vector<ReadWriteTexture*>& textures) : bound(false) {
    this->textures = textures;

    // Frame buffer object.
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

    GLint attachment = GL_COLOR_ATTACHMENT0;
    for (std::size_t i = 0; i < textures.size(); ++i) {
        if (textures[i]->GetFormat() == GL_DEPTH_COMPONENT) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[i]->GetTexture(), 0);
        } else {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment++, GL_TEXTURE_2D, textures[i]->GetTexture(), 0);
        }
    }

    // Create and intialize draw buffers 
    std::vector<GLenum> drawBuffers(textures.size());
    for (std::size_t i = 0; i < textures.size(); i++)
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Framebuffer creation failed\n";

    // Default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &frameBufferObject);
}

void FrameBuffer::Bind() {
    if (bound) {
        Log() << "StorageBuffer::Bind Warning: Already bound.\n";
        return;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferObject);

    bound = true;
}

void FrameBuffer::Unbind() {
    if (!bound) {
        Log() << "StorageBuffer::Bind Warning: Not bound.\n";
        return;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    bound = false;
}

void FrameBuffer::Clear() const {
    assert(bound);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
