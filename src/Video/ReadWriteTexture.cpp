#include "ReadWriteTexture.hpp"

#include <Utility/Log.hpp>

using namespace Video;

ReadWriteTexture::ReadWriteTexture(const glm::vec2& size, FORMAT format) {
    this->format = format;
    this->size = size;
    const unsigned int width = static_cast<int>(size.x);
    const unsigned int height = static_cast<int>(size.y);
    
    switch (format)
    {
    case RGB8:
        CreateTexture(texture, width, height, GL_RGB, GL_RGB);
        break;
    case RGB16:
        CreateTexture(texture, width, height, GL_RGB, GL_RGB16F);
        break;
    case DEPTH32:
        CreateTexture(texture, width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32);
        break;
    default:
        assert(false, "Read/write texture creation failed: Unsupported format\n");
    }
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

ReadWriteTexture::FORMAT ReadWriteTexture::GetFormat() const {
    return format;
}

void ReadWriteTexture::CreateTexture(GLuint& texture, unsigned width, unsigned height, GLenum format, GLint internalFormat) {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (format == GL_DEPTH_COMPONENT)
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glBindTexture(GL_TEXTURE_2D, 0);
}
