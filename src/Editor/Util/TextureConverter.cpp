#include "TextureConverter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

#include <Utility/Log.hpp>
#include <Video/Texture/TextureHCT.hpp>

namespace TextureConverter {
    void Convert(const char* inFilename, const char* outFilename) {
        // Load PNG file.
        int components, width, height;
        unsigned char* data = stbi_load(inFilename, &width, &height, &components, 0);
        if (data == NULL) {
            Log(Log::ERR) << "Couldn't load image: " << inFilename << "\n";
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
        
        /// @todo Store results to file.
        
        delete[] rgbData;
    }
}
