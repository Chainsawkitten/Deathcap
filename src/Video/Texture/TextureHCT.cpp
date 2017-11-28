#include "TextureHCT.hpp"

#include <fstream>
#include <Utility/Log.hpp>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Video;

TextureHCT::TextureHCT(const char* filename, uint16_t textureReduction) {
    // Open file for reading.
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        Log(Log::ERR) << "Couldn't open texture: " << filename << ".\n" <<
                         "Try reimporting the texture.\n";
        return;
    }
    
    // Check that version number is correct.
    uint16_t version;
    file.read(reinterpret_cast<char*>(&version), sizeof(uint16_t));
    if (version != VERSION) {
        Log(Log::ERR) << filename << " has the wrong version number.\n" <<
                         "Has " << version << ", should be " << VERSION << "\n" <<
                         "Try reimporting the texture.\n";
        file.close();
        return;
    }
    
    // Read other header information.
    uint16_t width, height, mipLevels;
    file.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&mipLevels), sizeof(uint16_t));
    
    // We can't load a smaller mip level if there are none.
    if (textureReduction >= mipLevels)
        textureReduction = mipLevels - 1;
    
    // Create image on GPU.
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexStorage2D(GL_TEXTURE_2D, mipLevels - textureReduction, GL_RGB8, width >> textureReduction, height >> textureReduction);
    
    // Read texture data.
    uint32_t size = static_cast<uint32_t>(width) * height * 3;
    unsigned char* data = new unsigned char[size];
    for (uint16_t mipLevel = 0; mipLevel < mipLevels; ++mipLevel) {
        size = static_cast<uint32_t>(width) * height * 3;
        if (!file.read(reinterpret_cast<char*>(data), size)) {
            Log(Log::ERR) << "Couldn't read data from texture file: " << filename << "\n";
            file.close();
            delete[] data;
            return;
        }
        
        if (mipLevel >= textureReduction)
            glTexSubImage2D(GL_TEXTURE_2D, mipLevel - textureReduction, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        width /= 2;
        height /= 2;
    }
    
    // Close file when finished reading.
    file.close();
    
    delete[] data;
    
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Repeat texture when texture coordinates outside 0.0-1.0.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    loaded = true;
}

TextureHCT::~TextureHCT() {
    if (texID != 0)
        glDeleteTextures(1, &texID);
}

GLuint TextureHCT::GetTextureID() const {
    return texID;
}

bool TextureHCT::IsLoaded() const {
    return loaded;
}
