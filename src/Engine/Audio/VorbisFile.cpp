#include "VorbisFile.hpp"

#include <Utility/Log.hpp>
#include <stb_vorbis.c>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Audio;

Audio::VorbisFile::VorbisFile(){

}

VorbisFile::~VorbisFile() {
    stb_vorbis_close(stbFile);
}

int VorbisFile::GetData(uint32_t offset, uint32_t samples, float* data) const {
    if (stbFile == nullptr)
        return 0;

    stb_vorbis_seek(stbFile, offset);
    return stb_vorbis_get_samples_float_interleaved(stbFile, channelCount, data, samples);
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
    if (error == VORBIS_file_open_failure) {
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
        stbFile = nullptr;
    } else {
        stb_vorbis_info stbInfo = stb_vorbis_get_info(stbFile);
        channelCount = stbInfo.channels;
        sampleRate = stbInfo.sample_rate;
        sampleCount = stb_vorbis_stream_length_in_samples(stbFile) * stbInfo.channels;
    }
}
