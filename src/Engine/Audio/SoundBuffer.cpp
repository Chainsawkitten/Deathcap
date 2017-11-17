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

SoundBuffer::SoundBuffer() {

}

SoundBuffer::~SoundBuffer() {

}

float* SoundBuffer::GetChunkData(int& samples) {
    if (!soundFile) {
        Log() << "SoundBuffer::GetBuffer: No sound loaded.\n";
        return nullptr;
    }

    assert(!chunkQueue.empty());

    SoundStreamer::DataHandle& handle = chunkQueue.front();
    while (!handle.done);

    if (handle.samples < CHUNK_SIZE)
        std::memset(&handle.data[(handle.offset + handle.samples) % (CHUNK_SIZE * CHUNK_COUNT)], 0, sizeof(CHUNK_SIZE - handle.samples));

    samples = handle.samples;
    return handle.data;
}

void SoundBuffer::ConsumeChunk() {
    chunkQueue.pop();
}

void SoundBuffer::ProduceChunk() {
    chunkQueue.push(Audio::SoundStreamer::DataHandle(soundFile, begin, CHUNK_SIZE, &buffer[begin % (CHUNK_SIZE * CHUNK_COUNT)]));
    Managers().soundManager->Load(chunkQueue.back());
    begin += CHUNK_SIZE;
}

SoundFile* SoundBuffer::GetSoundFile() const {
    return soundFile;
}

void SoundBuffer::SetSoundFile(SoundFile* soundFile) {
    begin = 0;
    Managers().soundManager->Flush(chunkQueue);
    this->soundFile = soundFile;
    assert(chunkQueue.empty());
    for (int i = 0; i < CHUNK_COUNT; ++i)
        ProduceChunk();
}

void SoundBuffer::Restart() {
    begin = 0;
    Managers().soundManager->Flush(chunkQueue);
    for (int i = 0; i < CHUNK_COUNT; ++i)
        ProduceChunk();
}
