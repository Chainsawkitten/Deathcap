#include "VorbisFile.hpp"

#include <Utility/Log.hpp>
#include <stb_vorbis.c>

using namespace Audio;

Audio::VorbisFile::VorbisFile()
{

}

VorbisFile::~VorbisFile() {
    stb_vorbis_close(stbFile);
}

void VorbisFile::GetData(uint32_t offset, uint32_t& samples, float* data) const {
    stb_vorbis_seek(stbFile, offset);
    samples = stb_vorbis_get_samples_float_interleaved(stbFile, channelCount, data, samples);
}

uint32_t VorbisFile::GetSampleCount() const {
    return sampleCount;
}

uint32_t VorbisFile::GetSampleRate() const {
    return sampleRate;
}

uint32_t VorbisFile::GetChannelCount() const {
    return channelCount;
}

void VorbisFile::Load(const char* filename) {

    // Open OGG file
    int error;
    stbFile = stb_vorbis_open_filename(filename, &error, NULL);
    if (error == VORBIS_file_open_failure)
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
    else {
        stb_vorbis_info stbInfo = stb_vorbis_get_info(stbFile);
        channelCount = stbInfo.channels;
        sampleRate = stbInfo.sample_rate;
        sampleCount = stb_vorbis_stream_length_in_samples(stbFile) * stbInfo.channels;
    }
}
