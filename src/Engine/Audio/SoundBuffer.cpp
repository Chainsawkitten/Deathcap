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

int SoundBuffer::GetData(float* data, unsigned int samples) {
    if (!buffer) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        data = nullptr;
        return 0;
    }

    assert(samples <= bufferSampleCount);

    int validSamples = samples;
    // Load slice if fetching data outside buffer.
    if (currentSample + samples > endSample) {
        // Reset buffer.
        std::memset(buffer, 0, bufferSampleCount * sizeof(float));

        // Fetch slice to put in buffer.
        beginSample = currentSample;
        validSamples = soundFile->GetData(beginSample, samples, buffer);
        endSample = beginSample + validSamples;
    }

    // Set data pointer in buffer.
    int index = currentSample - beginSample;
    assert(index >= 0 && index < bufferSampleCount);
    data = &buffer[index];

    // Increment current sample.
    currentSample += validSamples;

    // Return number of valied samples.
    return validSamples;
}

SoundFile* SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void SoundBuffer::SetSoundFile(SoundFile* soundFile) {
    this->soundFile = soundFile;
    const float bufferTime = 1.f;
    bufferSampleCount = soundFile->GetSampleRate() * soundFile->GetChannelCount() * bufferTime;
    if (buffer)
        delete buffer;
    buffer = new float[bufferSampleCount];
}

void SoundBuffer::Restart() {
    currentSample = 0;
    beginSample = 0;
    endSample = 0;
}
