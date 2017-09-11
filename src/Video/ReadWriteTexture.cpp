#include "ReadWriteTexture.hpp"

#include <Utility/Log.hpp>

using namespace Video;

ReadWriteTexture::ReadWriteTexture(const glm::vec2& size, FORMAT format) {
    this->size = size;
    const unsigned int width = static_cast<int>(size.x);
    const unsigned int height = static_cast<int>(size.y);
    
    // Format.
    GLint glInternalFormat = NULL;
    GLenum glFormat = NULL;
    switch (format)
    {
    case RGBA8:
        glInternalFormat = GL_RGBA;
        glFormat = GL_RGBA;
        break;
    case RGB16:
        glInternalFormat = GL_RGB16F;
        glFormat = GL_RGB;
        break;
    default:
        Log() << "Read/write texture creation failed: Unsupported format\n";
    }

    // Create texture.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ReadWriteTexture::~ReadWriteTexture() {
    glDeleteTextures(1, &texture);
}

glm::vec2 ReadWriteTexture::GetSize() const {
    return size;
}

GLuint ReadWriteTexture::GetTexture() const {
    return texture;
}
