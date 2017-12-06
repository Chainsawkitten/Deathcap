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
#include <cstring>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Audio;

SoundBuffer::SoundBuffer() {

}

SoundBuffer::~SoundBuffer() {
    SetSoundFile(nullptr);
}

float* SoundBuffer::GetChunkData(int& samples) {
    if (!soundFile) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        samples = 0;
        return nullptr;
    }

    if (chunkQueue.Empty()) {
        samples = 0;
        return nullptr;
    }

    SoundStreamer::DataHandle* handle = chunkQueue.Front();
    while (handle->abort) {
        while (!handle->done)
            Log() << "SoundBuffer::GetChunkData(" << soundFile->name << "): Blocking, chunk aborted but not done!\n";
        chunkQueue.Pop();
        if (chunkQueue.Empty()) {
            samples = 0;
            return nullptr;
        }
        handle = chunkQueue.Front();
    }

    while (!handle->done)
        Log() << "SoundBuffer::GetChunkData(" << soundFile->name << "): Blocking, chunk not done!\n";
    
    samples = handle->samples;
    return handle->data;
}

void SoundBuffer::ConsumeChunk() {
    assert(soundFile);
    assert(!chunkQueue.Empty());
    chunkQueue.Pop();
}

void SoundBuffer::ProduceChunk() {
    assert(soundFile);
    if (begin < soundFile->GetSampleCount()) {
        chunkQueue.Push(SoundStreamer::DataHandle(soundFile, begin, CHUNK_SIZE, &buffer[begin % (CHUNK_SIZE * chunkCount)]));
        Managers().soundManager->Load(chunkQueue.Back());
        begin += CHUNK_SIZE;
    }
}

SoundFile* SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void SoundBuffer::SetSoundFile(SoundFile* soundFile) {

    // Remove old sound file.
    if (this->soundFile) {
        begin = 0;
        Managers().soundManager->Flush(chunkQueue, true);
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
    }

    // Update sound file.
    this->soundFile = soundFile;

    // Set new sound file.
    if (soundFile) {
        chunkCount = soundFile->GetCached() ? 1 : CHUNK_COUNT;
        buffer = new float[CHUNK_SIZE * chunkCount];
        for (int i = 0; i < chunkCount; ++i)
            ProduceChunk();
    }
}

void SoundBuffer::Restart() {
    assert(soundFile);
    begin = 0;
    Managers().soundManager->Flush(chunkQueue);
    for (int i = 0; i < chunkCount - 1; ++i)
        ProduceChunk();
}
