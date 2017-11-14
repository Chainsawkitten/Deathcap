#include "SoundBuffer.hpp"

#include <algorithm>
#include "../Manager/SoundManager.hpp"
#include "SoundFile.hpp"
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include "VorbisFile.hpp"
#include <Utility/Log.hpp>

using namespace Audio;

Audio::SoundBuffer::SoundBuffer() {

}

SoundBuffer::SoundBuffer(SoundFile* soundFile) {
    SetSoundFile(soundFile);
}

SoundBuffer::~SoundBuffer() {
    if (buffer)
        delete buffer;
}

void SoundBuffer::GetBuffer(float* data, uint32_t& samples) {
    if (!buffer) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        data = nullptr;
        samples = 0;
        return;
    }

    soundFile->GetData(currentSample, samples, buffer);
    data = &buffer[0];
    currentSample += samples;
}

SoundFile* Audio::SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void Audio::SoundBuffer::SetSoundFile(SoundFile* soundFile) {
    this->soundFile = soundFile;
    const float bufferTime = 1.f;
    bufferSampleCount = soundFile->GetSampleRate() * soundFile->GetChannelCount() * bufferTime;
    buffer = new float[bufferSampleCount];
}
