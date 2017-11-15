#include "SoundStreamer.hpp"
#include "../Manager/SoundManager.hpp"
#include "SoundBuffer.hpp"
#include "SoundFile.hpp"
#include "../Component/SoundSource.hpp"

using namespace Audio;

SoundStreamer::SoundStreamer(SoundManager* soundManager) {
    thread = std::thread(std::bind(&SoundStreamer::Run, this, soundManager));
    lock = std::unique_lock<std::mutex>(mutex, std::defer_lock);
}

SoundStreamer::~SoundStreamer() {
    done = true;
    thread.join();
}

void SoundStreamer::Run(SoundManager* soundManager) {
    while (!done)
    {
        lock.lock();
        const std::vector<Component::SoundSource*>& soundSources = soundManager->GetSoundSources();
        for (Component::SoundSource* soundSource : soundSources) {
            SoundBuffer* soundBuffer = soundSource->soundBuffer;
            // Skip if sound buffer doesn't have a sound file.
            SoundFile* soundFile = soundBuffer->GetSoundFile();
            if (!soundFile)
                continue;

            unsigned int samplesToLoad = soundBuffer->bufferSampleCount - (soundBuffer->end - soundBuffer->begin);

            // Skip if buffer already is full.
            if (samplesToLoad == 0)
                continue;

            // Load chunks from file.
            unsigned int index = soundBuffer->end % soundBuffer->bufferSampleCount;
            int samplesLoaded = soundFile->GetData(soundBuffer->end, samplesToLoad, &soundBuffer->buffer[index]);
            
            // If end of file, add silence.
            if (samplesLoaded < samplesToLoad)
                std::memset(&soundBuffer->buffer[index + samplesLoaded], 0, (samplesToLoad - samplesLoaded) * sizeof(float));
            soundBuffer->end += samplesToLoad;
        }
        lock.unlock();
        std::this_thread::yield();
    }
}
