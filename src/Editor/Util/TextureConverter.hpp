#pragma once

#include <Video/Texture/TextureHCT.hpp>

/// Functions for texture conversion.
namespace TextureConverter {
    /// Convert texture from PNG to HCT format.
    /**
     * @param inFilename Filename of PNG file.
     * @param outFilename Filename of HCT file.
     * @param compressionType Which compression type to use.
     */
    void Convert(const char* inFilename, const char* outFilename, Video::TextureHCT::CompressionType compressionType);
}
