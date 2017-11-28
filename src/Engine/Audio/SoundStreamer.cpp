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


void SoundStreamer::Load(SoundStreamer::DataHandle& dataHandle) {
    if (dataHandle.soundFile->GetCached()) {
        dataHandle.samples = dataHandle.soundFile->GetData(dataHandle.offset, dataHandle.samples, dataHandle.data);
        dataHandle.done = true;
    } else {
        std::unique_lock<std::mutex> lock(queueMutex, std::defer_lock);
        lock.lock();
        loadQueue.push(&dataHandle);
        lock.unlock();
    }
}

void SoundStreamer::Flush(std::queue<DataHandle>& queue) {
    std::unique_lock<std::mutex> flushLock(flushMutex, std::defer_lock);
    flushLock.lock();
    while (!queue.empty()) {
        DataHandle& handle = queue.front();
        handle.abort = true;
        queue.pop();
    }
    flushLock.unlock();
}

void SoundStreamer::Worker::Start(SoundStreamer* soundStreamer) {
    workThread = std::thread(std::bind(&SoundStreamer::Worker::Execute, this, soundStreamer));
}

void SoundStreamer::Worker::Execute(SoundStreamer* soundStreamer) {
    while (!soundStreamer->stopWorker) {
        std::unique_lock<std::mutex> flushLock(soundStreamer->flushMutex, std::defer_lock);
        flushLock.lock();
        // Check queue.
        if (!soundStreamer->loadQueue.empty()) {
            std::unique_lock<std::mutex> queueLock(soundStreamer->queueMutex, std::defer_lock);
            queueLock.lock();
            DataHandle* handle = soundStreamer->loadQueue.front();
            soundStreamer->loadQueue.pop();
            queueLock.unlock();

            if (!handle->abort) {
                // Load data from file.
                handle->samples = handle->soundFile->GetData(handle->offset, handle->samples, handle->data);
                handle->done = true;
            }
        }
        flushLock.unlock();
        std::this_thread::yield();
    }
}

void SoundStreamer::Worker::Join() {
    workThread.join();
}
