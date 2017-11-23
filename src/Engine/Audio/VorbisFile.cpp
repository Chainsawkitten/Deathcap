#include "VorbisFile.hpp"

#include <algorithm>
#include <stb_vorbis.c>
#include <Utility/Log.hpp>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Audio;

Audio::VorbisFile::VorbisFile()
{

}

VorbisFile::~VorbisFile() {
    if (stbFile)
        stb_vorbis_close(stbFile);

    if (buffer)
        delete[] buffer;
}

int VorbisFile::GetData(uint32_t offset, uint32_t samples, float*& data) const {
    if (buffer) {
        data = &buffer[offset];
        return std::min(std::max(sampleCount - (int)(offset + samples), 0), (int)samples);
    } else {
        stb_vorbis_seek(stbFile, offset);
        return stb_vorbis_get_samples_float_interleaved(stbFile, channelCount, data, samples);
    }
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

void VorbisFile::Cache(bool cache) {
    if (!stbFile) {
        Log() << "No OGG Vorbis file loaded to cache.\n";
        return;
    }

    if (cache && !buffer) {
        buffer = new float[sampleCount];
        stb_vorbis_get_samples_float_interleaved(stbFile, channelCount, buffer, sampleCount);
    } else if (!cache && buffer) {
        delete[] buffer;
        buffer = nullptr;
    }
}

void VorbisFile::Load(const char* filename) {

    // Open OGG file
    int error;
    stbFile = stb_vorbis_open_filename(filename, &error, NULL);
    if (!stbFile) {
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
        return;
    }

    stb_vorbis_info stbInfo = stb_vorbis_get_info(stbFile);
    channelCount = stbInfo.channels;
    sampleRate = stbInfo.sample_rate;
    sampleCount = stb_vorbis_stream_length_in_samples(stbFile) * stbInfo.channels;
}
