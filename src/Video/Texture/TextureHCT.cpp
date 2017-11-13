#include "TextureHCT.hpp"

#include <stb_image.h>
#include <Utility/Log.hpp>

using namespace Video;

TextureHCT::TextureHCT(const char* filename) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Load texture from file.
    int components, width, height;
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

TextureHCT::~TextureHCT() {
    
}

GLuint TextureHCT::GetTextureID() const {
    return texID;
}

bool TextureHCT::IsLoaded() const {
    return loaded;
}
