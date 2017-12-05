#include "SoundStreamer.hpp"
#include "../Manager/SoundManager.hpp"
#include "SoundBuffer.hpp"
#include "SoundFile.hpp"
#include "../Component/SoundSource.hpp"

using namespace Audio;

SoundStreamer::SoundStreamer() {
    worker.Start(this);
}

SoundStreamer::~SoundStreamer() {
    stopWorker = true;
    worker.Join();
}

SoundStreamer::DataHandle::DataHandle(SoundFile* soundFile, uint32_t offset, uint32_t samples, float* data) {
    this->soundFile = soundFile;
    this->offset = offset;
    this->samples = samples;
    this->data = data;
    done = false;
    abort = false;
}

void SoundStreamer::Load(SoundStreamer::DataHandle* handle) {
    assert(!handle->soundFile->GetCached());
    std::unique_lock<std::mutex> lock(queueMutex, std::defer_lock);
    lock.lock();
    loadQueue.Push(handle);
    lock.unlock();
}

void SoundStreamer::BeginFlush() {
    flushLock = std::unique_lock<std::mutex>(flushMutex, std::defer_lock);
    flushLock.lock();
}


void SoundStreamer::EndFlush() {
    flushLock.unlock();
}


void SoundStreamer::Worker::Start(SoundStreamer* soundStreamer) {
    workThread = std::thread(std::bind(&SoundStreamer::Worker::Execute, this, soundStreamer));
}

void SoundStreamer::Worker::Execute(SoundStreamer* soundStreamer) {
    while (!soundStreamer->stopWorker) {
        // Pop work from queue while work is available.
        while (!soundStreamer->loadQueue.Empty()) {
            // Get work from queue.
            std::unique_lock<std::mutex> queueLock(soundStreamer->queueMutex, std::defer_lock);
            queueLock.lock();
            DataHandle* handle = *soundStreamer->loadQueue.Front();
            soundStreamer->loadQueue.Pop();
            queueLock.unlock();

            // Load data from file.
            if (!handle->abort) {
                std::unique_lock<std::mutex> lock(soundStreamer->flushMutex, std::defer_lock);
                lock.lock();
                assert(handle->offset < handle->soundFile->GetSampleCount());
                handle->samples = handle->soundFile->GetData(handle->offset, handle->samples, handle->data);
                lock.unlock();
            }
            handle->done = true;
        }
        // Sleep when thread is idle.
        std::this_thread::yield();
    }
}

void SoundStreamer::Worker::Join() {
    workThread.join();
}
