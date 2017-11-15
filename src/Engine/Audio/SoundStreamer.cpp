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


        //while (!done) {

        //    // Check queue.
        //    if (!loadQueue.empty()) {
        //        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        //        lock.lock();
        //        DataHandle* handle = loadQueue.front();
        //        loadQueue.pop();
        //        lock.unlock();

        //        // Load data from file.
        //        handle->samples = handle->soundFile->GetData(handle->offset, handle->samples, handle->data);

        //        handle->done = true;
        //    }

            // Yield of queue is empty.
            //if (loadQueue.empty())


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

        std::this_thread::yield();
    }
}

void SoundStreamer::Worker::Join() {
    workThread.join();
}
