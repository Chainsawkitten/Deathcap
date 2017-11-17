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
}

bool SoundStreamer::DataHandle::Done() const {
    return done;
}

void SoundStreamer::Load(SoundStreamer::DataHandle& dataHandle) {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    lock.lock();
    loadQueue.push(&dataHandle);
    lock.unlock();
}

void SoundStreamer::Worker::Start(SoundStreamer* soundStreamer) {
    workThread = std::thread(std::bind(&SoundStreamer::Worker::Execute, this, soundStreamer));
}

void SoundStreamer::Worker::Execute(SoundStreamer* soundStreamer) {
    while (!soundStreamer->stopWorker) {
        // Check queue.
        if (!soundStreamer->loadQueue.empty()) {
            std::unique_lock<std::mutex> lock(soundStreamer->mutex, std::defer_lock);
            lock.lock();
            DataHandle* handle = soundStreamer->loadQueue.front();
            soundStreamer->loadQueue.pop();
            lock.unlock();

            // Load data from file.
            handle->samples = handle->soundFile->GetData(handle->offset, handle->samples, handle->data);

            handle->done = true;
        }
        std::this_thread::yield();
    }
}

void SoundStreamer::Worker::Join() {
    workThread.join();
}
