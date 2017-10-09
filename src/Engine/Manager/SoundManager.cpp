#include "SoundManager.hpp"
#include <phonon.h>
#include <Utility/Log.hpp>
#include <AL/al.h>
#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Listener.hpp"
#include "../Component/SoundSource.hpp"
#include "../Component/Physics.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include "portaudio.h"

#define SAMPLE_RATE (44100)
#define PA_SAMPLE_TYPE  paInt16

SoundManager::SoundManager() {
    PaError err;

    // Initialize PortAudio
    err = Pa_Initialize();
    CheckError(err);
    
    PaStreamParameters outputParams;
    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = 1;
    outputParams.sampleFormat = PA_SAMPLE_TYPE;
    outputParams.hostApiSpecificStreamInfo = NULL;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultHighOutputLatency;

    // Open Stream
    err = Pa_OpenStream(
        &stream,
        NULL,
        &outputParams,
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,
        paClipOff,
        NULL,
        NULL
    );
    CheckError(err);

    processedFrameSamples = new short[735] { 0 };

    err = Pa_StartStream(stream);
    CheckError(err);
}


SoundManager::~SoundManager() {
    delete processedFrameSamples;
    Pa_CloseStream(stream);
    Pa_Terminate();
}

void SoundManager::CheckError(PaError err) {
    if (err != paNoError) {
        Pa_Terminate();
        Log() << "An error occured while using the portaudio stream\n";
        Log() << "Error number:" << err << "\n";
        Log() << "Error message: " << Pa_GetErrorText(err) << "\n";
    }
}
bool firstframe = true;
void SoundManager::Update(float deltaTime) {
    
    // Number of samples to process dependant on deltaTime
    int numSamples = SAMPLE_RATE * deltaTime;

    // Update sound sources.
    for (Component::SoundSource* sound : soundSources.GetAll()) {
        // Obviously send all buffers into steam audio and use the mix from there to fill processedFrameSamples. The solution below only works properly with one soundsource.
        if (sound->shouldPlay) {
            delete processedFrameSamples;
            processedFrameSamples = new short[numSamples];
            if (sound->soundBuffer->GetSize() > sound->place + sizeof(char)*numSamples) {
                short* audioBuffer = sound->soundBuffer->GetBuffer();
                memcpy(processedFrameSamples,(sound->soundBuffer->GetBuffer() + sound->place), sizeof(short)*numSamples);
                sound->place += numSamples;
            } else {
                // Only copy the end samples of the buffer
                size_t numToCpy = numSamples - (sound->soundBuffer->GetSize() - sound->place)/sizeof(short);
                memcpy(processedFrameSamples, (sound->soundBuffer->GetBuffer() + sound->place), numToCpy);
                if (sound->loop) {
                    memcpy(processedFrameSamples + numToCpy*sizeof(short), sound->soundBuffer->GetBuffer(), sizeof(short)*numSamples -numToCpy);
                    sound->place = numSamples - numToCpy;
                } else {
                    memset(processedFrameSamples + numToCpy * sizeof(short), 0, sizeof(short)*(numSamples - numToCpy));
                    sound->shouldPlay = false;
                }
            }
        }

        /*if (sound->IsKilled() || !sound->entity->enabled)
            continue;
        
        Entity* entity = sound->entity;
        
        // Pause it.
        if (sound->shouldPause) {
            alSourcePause(sound->source);
            sound->shouldPause = false;
        }
        
        // Stop it.
        if (sound->shouldStop) {
            alSourceStop(sound->source);
            sound->shouldStop = false;
        }
        
        // Set position.
        glm::vec3 position = soundScale * glm::vec3(entity->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f));
        alSource3f(sound->source, AL_POSITION, position.x, position.y, position.z);
        
        // Set velocity based on physics.
        Component::Physics* physics = entity->GetComponent<Component::Physics>();
        if (physics != nullptr) {
            glm::vec3 velocity = soundScale * physics->velocity;
            alSource3f(sound->source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        } else {
            alSource3f(sound->source, AL_VELOCITY, 0.f, 0.f, 0.f);
        }
        
        // Set other properties.
        alSourcef(sound->source, AL_PITCH, sound->pitch);
        alSourcef(sound->source, AL_GAIN, sound->gain);
        alSourcei(sound->source, AL_LOOPING, sound->loop);
        if (sound->soundBuffer != nullptr && !sound->soundBufferSet) {
            alSourcei(sound->source, AL_BUFFER, sound->soundBuffer->GetBuffer());
            sound->soundBufferSet = true;
        }

        // Play it.
        if (sound->shouldPlay) {
            alSourcePlay(sound->source);
            sound->shouldPlay = false;
        }
        
        CheckError("Something went wrong updating a sound source.");
        */
    }
    
    // Update listener.
    for (Component::Listener* listener : listeners.GetAll()) {
        Entity* entity = listener->entity;
        
        // Set position
        //glm::vec3 position = soundScale * entity->position;
        //alListener3f(AL_POSITION, position.x, position.y, position.z);
        
        // Set orientation.
        glm::vec4 forward = glm::inverse(entity->GetOrientation()) * glm::vec4(0.f, 0.f, -1.f, 1.f);
        glm::vec4 up = glm::inverse(entity->GetOrientation()) * glm::vec4(0.f, 1.f, 0.f, 1.f);
        ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
        alListenerfv(AL_ORIENTATION, listenerOri);
        
        break;
    }

    Pa_WriteStream(stream, processedFrameSamples, numSamples);
}

Component::SoundSource* SoundManager::CreateSoundSource() {
    return soundSources.Create();
}

Component::SoundSource* SoundManager::CreateSoundSource(const Json::Value& node) {
    Component::SoundSource* soundSource = soundSources.Create();
    
    // Load values from Json node.
    std::string name = node.get("sound", "").asString();
    if (!name.empty())
        soundSource->soundBuffer = Managers().resourceManager->CreateSound(name);
    
    soundSource->pitch = node.get("pitch", 1.f).asFloat();
    soundSource->gain = node.get("gain", 1.f).asFloat();
    soundSource->loop = node.get("loop", false).asBool();
    
    return soundSource;
}

const std::vector<Component::SoundSource*>& SoundManager::GetSoundSources() const {
    return soundSources.GetAll();
}

Component::Listener* SoundManager::CreateListener() {
    return listeners.Create();
}

Component::Listener* SoundManager::CreateListener(const Json::Value& node) {
    return listeners.Create();
}

const std::vector<Component::Listener*>& SoundManager::GetListeners() const {
    return listeners.GetAll();
}

void SoundManager::ClearKilledComponents() {
    soundSources.ClearKilled();
    listeners.ClearKilled();
}
