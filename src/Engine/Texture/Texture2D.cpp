#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#include <stb_image.h>

#include <Common/Log.hpp>
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"

using namespace Video;

Texture2D::Texture2D() {
    isFromFile = false;
}

Texture2D::Texture2D(const char* filename, bool srgb) {
    Load(filename, srgb);
}

Texture2D::Texture2D(const char *source, int sourceLength, bool srgb) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(source), sourceLength, &width, &height, &components, 0);
    
    if (data == NULL)
        Log() << "Couldn't load headerized image.\n";
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB_ALPHA : GL_RGBA, width, height, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
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
    
    isFromFile = false;
    this->srgb = srgb;
    loaded = true;
}

Texture2D::~Texture2D() {
    if (texID != 0)
        glDeleteTextures(1, &texID);
}

GLuint Texture2D::GetTextureID() const {
    return texID;
}

int Texture2D::GetWidth() const {
    return width;
}

int Texture2D::GetHeight() const {
    return height;
}

void Texture2D::SetWrapping(GLint wrapMode) {
    glBindTexture(GL_TEXTURE_2D, texID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

bool Texture2D::IsFromFile() const {
    return isFromFile;
}

Json::Value Texture2D::Save() const {
    Json::Value texture;
    texture["name"] = name;
    texture["srgb"] = srgb;
    return texture;
}

void Texture2D::Load(const Json::Value& node) {
    name = node.get("name", "").asString();
    Load((Hymn().GetPath() + FileSystem::DELIMITER + "Textures" + FileSystem::DELIMITER + name + ".png").c_str(), node.get("srgb", false).asBool());
}

void Texture2D::Load(const char* filename, bool srgb) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Load texture from file.
    int components;
    unsigned char* data = stbi_load(filename, &width, &height, &components, 0);
    
    if (data == NULL)
        Log() << "Couldn't load image " << filename << "\n";
    
    // Give the image to OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB_ALPHA : GL_RGBA, width, height, 0, Format(components), GL_UNSIGNED_BYTE, data);
    
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
    
    isFromFile = true;
    loaded = true;
    this->srgb = srgb;
}

bool Texture2D::IsLoaded() const {
    return loaded;
}
