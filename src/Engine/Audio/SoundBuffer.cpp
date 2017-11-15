#include "SoundBuffer.hpp"

#include <algorithm>
#include "../Manager/SoundManager.hpp"
#include "SoundFile.hpp"
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include "VorbisFile.hpp"
#include <Utility/Log.hpp>

using namespace Audio;

SoundBuffer::SoundBuffer(unsigned int samplesPerChunk, unsigned int chunkCount) : samplesPerChunk(samplesPerChunk), chunkCount(chunkCount) {
    bufferSampleCount = samplesPerChunk * chunkCount;
    buffer = new float[samplesPerChunk * chunkCount];
}

SoundBuffer::~SoundBuffer() {
    delete buffer;
}

int SoundBuffer::GetData(float* data, unsigned int samples) {
    if (!soundFile) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        return 0;
    }

    assert(begin + samples < end);
    data = &buffer[begin % bufferSampleCount];
    begin += samples;
}

SoundFile* SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void SoundBuffer::SetSoundFile(SoundFile* soundFile) {
    this->soundFile = soundFile;
}

void SoundBuffer::Restart() {
    begin = 0;
    end = 0;
}
