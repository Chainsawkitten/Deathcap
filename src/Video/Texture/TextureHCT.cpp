#include "TextureHCT.hpp"

#include <fstream>
#include <Utility/Log.hpp>
#include <cstring>
#include <miniz.h>

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
    uint16_t width, height, mipLevels, compressionType;
    file.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&mipLevels), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&compressionType), sizeof(uint16_t));
    
    // Read file contents.
    std::streampos currentPos = file.tellg();
    file.seekg(0, std::ios_base::end);
    unsigned int fileSize = file.tellg() - currentPos;
    file.seekg(currentPos);
    unsigned char* fileContents = new unsigned char[fileSize];
    if (!file.read(reinterpret_cast<char*>(fileContents), fileSize)) {
        Log(Log::ERR) << "Couldn't read data from texture file: " << filename << "\n";
        file.close();
        delete[] fileContents;
        return;
    }
    file.close();
    
    // Determine block size.
    GLenum format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    uint32_t blockSize = 8;
    switch (compressionType) {
    case BC1:
        format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        blockSize = 8;
        break;
    case BC4:
        format = GL_COMPRESSED_RED_RGTC1;
        blockSize = 8;
        break;
    case BC5:
        format = GL_COMPRESSED_RG_RGTC2;
        blockSize = 16;
        break;
    }
    
    // Allocate data buffer.
    unsigned int bufferSize = 0;
    uint16_t mWidth = width;
    uint16_t mHeight = height;
    for (uint16_t mipLevel = 0; mipLevel < mipLevels; ++mipLevel) {
        bufferSize += static_cast<uint32_t>(mWidth) * mHeight / 16 * blockSize;
        
        mWidth /= 2;
        mHeight /= 2;
    }
    unsigned char* buffer = new unsigned char[bufferSize];
    
    // Inflate decompression.
    mz_ulong destinationLength = bufferSize;
    int errCode = uncompress(buffer, &destinationLength, fileContents, fileSize);
    if (errCode != MZ_OK)
        Log(Log::ERR) << "Couldn't decompress: " << errCode << "\n";
    delete[] fileContents;
    
    // We can't load a smaller mip level if there are none.
    if (textureReduction >= mipLevels)
        textureReduction = mipLevels - 1;
    
    // Create image on GPU.
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexStorage2D(GL_TEXTURE_2D, mipLevels - textureReduction, format, width >> textureReduction, height >> textureReduction);
    
    // Transfer texture data.
    uint32_t size = static_cast<uint32_t>(width) * height / 16 * blockSize;
    unsigned char* data = new unsigned char[size];
    unsigned int bufferLocation = 0;
    for (uint16_t mipLevel = 0; mipLevel < mipLevels; ++mipLevel) {
        size = static_cast<uint32_t>(width) * height / 16 * blockSize;
        memcpy(data, &buffer[bufferLocation], size);
        bufferLocation += size;
        
        if (mipLevel >= textureReduction)
            glCompressedTexSubImage2D(GL_TEXTURE_2D, mipLevel - textureReduction, 0, 0, width, height, format, size, data);
        width /= 2;
        height /= 2;
    }
    
    delete[] buffer;
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
