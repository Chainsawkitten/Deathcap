#include "TextureConverter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

#include <Utility/Log.hpp>
#include <Video/Texture/TextureHCT.hpp>

namespace TextureConverter {
    void Convert(const char* inFilename, const char* outFilename) {
        // Load PNG file.
        /*int components, width, height;
        unsigned char* data = stbi_load(inFilename, &width, &height, &components, 0);
        if (data == NULL) {
            Log(Log::ERR) << "Couldn't load image: " << inFilename << "\n";
            return;
        }
        
        stbi_image_free(data);*/
        
        /// @todo Convert to HCT.
        
        /// @todo Store results to file.
    }
}
