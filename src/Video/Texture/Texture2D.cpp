#include "Texture2D.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Video;

Texture2D::Texture2D() {
    
}

Texture2D::~Texture2D() {
    
}

GLenum Texture2D::Format(int components) {
    switch (components) {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    default:
        return GL_RGBA;
    }
}
