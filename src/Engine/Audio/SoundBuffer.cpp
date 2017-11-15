#include "SoundBuffer.hpp"

#include <algorithm>
#include "../Manager/SoundManager.hpp"
#include "SoundFile.hpp"
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include "VorbisFile.hpp"
#include <Utility/Log.hpp>
#include "../Audio/SoundStreamer.hpp"
#include "../Manager/Managers.hpp"

using namespace Audio;

SoundBuffer::SoundBuffer(unsigned int samplesPerChunk, unsigned int chunkCount) : samplesPerChunk(samplesPerChunk), chunkCount(chunkCount) {
    bufferSampleCount = samplesPerChunk * chunkCount;
    buffer = new float[samplesPerChunk * chunkCount];
}

SoundBuffer::~SoundBuffer() {
    delete buffer;
}

void SoundBuffer::GetCurrentChunk(float* data) {
    if (!soundFile) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        return;
    }

    assert(!chunkQueue.empty());

    SoundStreamer::DataHandle& handle = chunkQueue.front();
    while (!handle.done) {}
    data = handle.data;


    //const std::vector<Component::SoundSource*>& soundSources = soundManager->GetSoundSources();
    //for (Component::SoundSource* soundSource : soundSources) {
    //    SoundBuffer* soundBuffer = soundSource->soundBuffer;

    //    // Skip if sound buffer doesn't have a sound file.
    //    SoundFile* soundFile = soundBuffer->GetSoundFile();
    //    if (!soundFile)
    //        continue;

    //    unsigned int samplesToLoad = soundBuffer->bufferSampleCount - (soundBuffer->end - soundBuffer->begin);

    //    // Skip if buffer already is full.
    //    if (samplesToLoad == 0)
    //        continue;

    //    // Load chunks from file.
    //    unsigned int index = soundBuffer->end % soundBuffer->bufferSampleCount;
    //    int samplesLoaded = soundFile->GetData(soundBuffer->end, samplesToLoad, &soundBuffer->buffer[index]);
    //    
    //    // If end of file, add silence.
    //    if (samplesLoaded < samplesToLoad)
    //        std::memset(&soundBuffer->buffer[index + samplesLoaded], 0, (samplesToLoad - samplesLoaded) * sizeof(float));
    //    soundBuffer->end += samplesToLoad;
    //}

    //unsigned int samplesToLoad = bufferSampleCount - (loadEnd - loadBegin);
    //if (samplesToLoad > 0) {
    //    unsigned int index = loadEnd % bufferSampleCount;
    //    Audio::SoundStreamer::DataHandle dataHandle;
    //    dataHandle.soundFile = soundFile;
    //    dataHandle.data = &buffer[index];
    //    dataHandle.offset = loadBegin;
    //    dataHandle.samples = samplesToLoad;
    //    Managers().soundManager->soundStreamer->Load(dataHandle);
    //    while (!dataHandle.done) {};
    //    return dataHandle.samples;
    //}


    //assert(begin + samples < end);
    //data = &buffer[begin % bufferSampleCount];
    //begin += samples;
}

void SoundBuffer::ConsumeChunk() {
    chunkQueue.pop();
    begin += samplesPerChunk;
    
    Audio::SoundStreamer::DataHandle dataHandle;
    dataHandle.soundFile = soundFile;
    dataHandle.data = &buffer[begin % bufferSampleCount];
    dataHandle.offset = end;
    dataHandle.samples = samplesPerChunk;
    chunkQueue.push(dataHandle);
    Managers().soundManager->soundStreamer->Load(chunkQueue.back());
    end += samplesPerChunk;
}

SoundFile* SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void SoundBuffer::SetSoundFile(SoundFile* soundFile) {
    this->soundFile = soundFile;
    assert(chunkQueue.empty());
    for (int i = 0; i < chunkCount; ++i) {
        Audio::SoundStreamer::DataHandle dataHandle;
        dataHandle.soundFile = soundFile;
        dataHandle.data = &buffer[i * samplesPerChunk];
        dataHandle.offset = i * samplesPerChunk;
        dataHandle.samples = samplesPerChunk;
        chunkQueue.push(dataHandle);
        Managers().soundManager->soundStreamer->Load(chunkQueue.back());
    }
}

void SoundBuffer::Restart() {
    begin = 0;
    end = 0;
}
