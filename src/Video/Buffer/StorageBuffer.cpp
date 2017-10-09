#include "StorageBuffer.hpp"

#include <assert.h>
#include <cstring>


#include <Utility/Log.hpp>
#define ERROR_CHECK_VIDEO { GLenum __err(glGetError()); while (__err != GL_NO_ERROR) { Log() << "GL error: " << (const char*)gluErrorString(__err) << "\n"; __err = glGetError(); assert(false); } }

using namespace Video;

StorageBuffer::StorageBuffer(unsigned int size, GLenum usage) : bound(false) {
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)size, NULL, usage);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    this->size = size;
}

StorageBuffer::~StorageBuffer() {
    glDeleteBuffers(1, &ssbo);
}

void StorageBuffer::Write(void* data, unsigned int offset, unsigned int length) {
    assert(bound);
    assert(this->size >= offset + length);

    //GLvoid* p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset, length, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT); ERROR_CHECK_VIDEO
    //GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); ERROR_CHECK_VIDEO
    //std::memcpy(p, &data, length);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, length, data); ERROR_CHECK_VIDEO //TODO MAP?

}

void StorageBuffer::Bind() {
    if (bound) {
        Log() << "StorageBuffer::Bind Warning: Already bound.\n";
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // TODO glBindBufferRange
    bound = true;
}

void StorageBuffer::Unbind() {
    if (!bound) {
        Log() << "StorageBuffer::Unbind Warning: Not bound.\n";
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    bound = false;
}

void StorageBuffer::BindBase(unsigned int binding) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)binding, ssbo); ERROR_CHECK_VIDEO
}

unsigned int StorageBuffer::GetSize() const {
    return size;
}
