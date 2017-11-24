#include "TextureConverter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

#include <fstream>
#include <Utility/Log.hpp>
#include <Codec_DXTC.h>
#include <GLFW/glfw3.h>

namespace TextureConverter {
    void Convert(const char* inFilename, const char* outFilename, Video::TextureHCT::CompressionType compressionType) {
        // Load PNG file.
        int components, width, height;
        unsigned char* data = stbi_load(inFilename, &width, &height, &components, 0);
        if (data == NULL) {
            Log(Log::ERR) << "Couldn't load image: " << inFilename << "\n";
            return;
        }
        if (width % 4 != 0 || height % 4 != 0) {
            Log(Log::ERR) << inFilename << " does not have dimensions multiple of 4.\n";
            return;
        }
        
        // Error on unsupported format.
        if (compressionType == Video::TextureHCT::BC4 || compressionType == Video::TextureHCT::BC5) {
            Log(Log::ERR) << "Unsupported compression type.\n";
            return;
        }
        
        // Convert to RGB (in case it's not already).
        unsigned char* rgbData = new unsigned char[width * height * 3];
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = y * width + x;
                rgbData[index * 3 + 0] = data[index * components + 0];
                rgbData[index * 3 + 1] = (components >= 2) ? data[index * components + 1] : 0;
                rgbData[index * 3 + 2] = (components >= 3) ? data[index * components + 2] : 0;
            }
        }
        
        stbi_image_free(data);
        
        // Open output file.
        std::ofstream file(outFilename, std::ios::out | std::ios::binary);
        if (!file) {
            Log(Log::ERR) << "Couldn't open file: " << outFilename << "\n";
            delete[] rgbData;
            return;
        }
        
        // Calculate the levels of mipmapping.
        uint16_t uWidth = width;
        uint16_t uHeight = height;
        uint16_t mipLevels;
        for (mipLevels = 0; width >= 4 && height >= 4; ++mipLevels) {
            width /= 2;
            height /= 2;
        }
        uint16_t type = Video::TextureHCT::BC1;
        
        // Write header.
        uint16_t version = Video::TextureHCT::VERSION;
        file.write(reinterpret_cast<const char*>(&version), sizeof(uint16_t));
        file.write(reinterpret_cast<const char*>(&uWidth), sizeof(uint16_t));
        file.write(reinterpret_cast<const char*>(&uHeight), sizeof(uint16_t));
        file.write(reinterpret_cast<const char*>(&mipLevels), sizeof(uint16_t));
        file.write(reinterpret_cast<const char*>(&type), sizeof(uint16_t));
        
        double time = glfwGetTime();
        
        // Write data.
        width = uWidth;
        height = uHeight;
        for (uint16_t mipLevel = 0; mipLevel < mipLevels; ++mipLevel) {
            // Compress using DXT1.
            uint16_t blocksX = width / 4;
            uint16_t blocksY = height / 4;
            for (uint16_t blockY = 0; blockY < blocksY; ++blockY) {
                for (uint16_t blockX = 0; blockX < blocksX; ++blockX) {
                    // Get uncompressed block.
                    uint8_t uncompressed[4 * 4 * 4];
                    for (uint8_t y=0; y < 4; ++y) {
                        for (uint8_t x=0; x < 4; ++x) {
                            for (uint8_t channel = 0; channel < 3; ++channel) {
                                uint32_t rgbY = blockY * 4 + y;
                                uint32_t rgbX = blockX * 4 + x;
                                uncompressed[(y * 4 + x) * 4 + channel] = rgbData[(rgbY * width + rgbX) * 3 + channel];
                            }
                            uncompressed[(y * 4 + x) * 4 + 3] = 255;
                        }
                    }
                    
                    // Convert block
                    uint32_t block[2];
                    CompressRGBBlock(uncompressed, block, CalculateColourWeightings(uncompressed), true, false, 255);
                    
                    // Write block to file.
                    file.write(reinterpret_cast<char*>(block), 8);
                }
            }
            
            // Calculate mipmap.
            if (mipLevel < mipLevels - 1) {
                width /= 2;
                height /= 2;
                
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        for (int component = 0; component < 3; ++component) {
                            uint16_t sum = 0;
                            sum += rgbData[(y * 2 * width * 2 + x * 2) * 3 + component];
                            sum += rgbData[(y * 2 * width * 2 + x * 2 + 1) * 3 + component];
                            sum += rgbData[((y * 2 + 1) * width * 2 + x * 2) * 3 + component];
                            sum += rgbData[((y * 2 + 1) * width * 2 + x * 2 + 1) * 3 + component];
                            rgbData[(y * width + x) * 3 + component] = sum / 4 + (sum % 4 > 1);
                        }
                    }
                }
            }
        }
        
        time = glfwGetTime() - time;
        Log(Log::INFO) << "Time to convert: " << time << "s\n";
        
        file.close();
        
        delete[] rgbData;
    }
}
