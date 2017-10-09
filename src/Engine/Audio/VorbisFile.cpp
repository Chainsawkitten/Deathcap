#include "VorbisFile.hpp"

#include <Utility/Log.hpp>
#include <stb_vorbis.c>

using namespace Audio;

VorbisFile::VorbisFile(const char *filename) {
    int channels;
    dataSize = stb_vorbis_decode_filename(filename, &channels, &sampleRate, &data);
    
    if (dataSize == -1)
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
    
    // We get size in samples, but we need it in bytes.
    dataSize *= channels * sizeof(short);
    
    if (channels > 1)
        format = AL_FORMAT_STEREO16;
    else
        format = AL_FORMAT_MONO16;
}

VorbisFile::~VorbisFile() {

}

short* VorbisFile::GetData() const {
    return data;
}

size_t VorbisFile::GetSize() const {
    return dataSize;
}

size_t VorbisFile::GetSampleRate() const {
    return sampleRate;
}
