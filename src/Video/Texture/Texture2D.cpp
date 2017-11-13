#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#include <stb_image.h>

#include <Utility/Log.hpp>

using namespace Video;

Texture2D::Texture2D() {
    
}

Texture2D::Texture2D(const char* source, int sourceLength) {
    Load(source, sourceLength);
}

Texture2D::~Texture2D() {
    if (texID != 0)
        glDeleteTextures(1, &texID);
}

GLuint Texture2D::GetTextureID() const {
    return texID;
}

void Texture2D::Load(const char* filename) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load(filename, &width, &height, &components, 0);
    
    if (data == NULL) {
        Log() << "Couldn't load image " << filename << "\n";
        loaded = false;
        return;
    }
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Repeat texture when texture coordinates outside 0.0-1.0.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Generate mipmaps.
    glGenerateMipmap(GL_TEXTURE_2D);
    
    loaded = true;
}

void Texture2D::Load(const char* source, int sourceLength) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(source), sourceLength, &width, &height, &components, 0);
    
    if (data == NULL)
        Log() << "Couldn't load headerized image.\n";
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Repeat texture when texture coordinates outside 0.0-1.0.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
    
    loaded = true;
}

bool Texture2D::IsLoaded() const {
    return loaded;
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
