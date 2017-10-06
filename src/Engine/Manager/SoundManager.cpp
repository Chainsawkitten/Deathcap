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
#define FRAMES_PER_BUFFER (735)
#define PA_SAMPLE_TYPE  paFloat32

// Scaling constant. Used to convert from our units to sound system units.
const float soundScale = 0.2f;
char* SoundManager::processedFrameSamples[2];

SoundManager::SoundManager() {
    
    // 
    PaStreamParameters outputParams;
    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = 2;
    outputParams.sampleFormat = PA_SAMPLE_TYPE;
    outputParams.hostApiSpecificStreamInfo = NULL;

    int sampleRate = SAMPLE_RATE;

    // Open Stream
    Pa_OpenStream(
        &stream,
        NULL,
        &outputParams,
        sampleRate,
        FRAMES_PER_BUFFER,
        0,
        fillOutputBufferCallback,
        NULL
    );

    processedFrameSamples[0] = new char[FRAMES_PER_BUFFER] { 0 };
    processedFrameSamples[1] = new char[FRAMES_PER_BUFFER] { 0 };
}

SoundManager::~SoundManager() {
    delete processedFrameSamples;
    Pa_CloseStream(stream);
    Pa_Terminate();
}

void SoundManager::CheckError(const char* message) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        Log() << message << "\n";
        if (error == AL_INVALID_NAME) Log() << "Invalid name\n";
        if (error == AL_INVALID_ENUM) Log() << "Invalid enum\n";
        if (error == AL_INVALID_VALUE) Log() << "Invalid value\n";
        if (error == AL_INVALID_OPERATION) Log() << "Invalid operation\n";
        if (error == AL_OUT_OF_MEMORY) Log() << "Out of memory like!\n";
    }
}

void SoundManager::Update() {
    // Update sound sources.
    for (Component::SoundSource* sound : soundSources.GetAll()) {

        // Obviously send all buffers into steam audio and use the mix from there to fill processedFrameSamples. The solution below only works properly with one soundsource.
        if (sound->shouldPlay) {
            delete processedFrameSamples[0];
            processedFrameSamples[0] = processedFrameSamples[1];
            processedFrameSamples[1] = new char[FRAMES_PER_BUFFER];
            if (sound->soundBuffer->GetSize() > sound->place + sizeof(char)*FRAMES_PER_BUFFER) {
                memcpy(processedFrameSamples[1],(sound->soundBuffer->GetBuffer() + sound->place), sizeof(char)*FRAMES_PER_BUFFER);
                sound->place += sizeof(char)*FRAMES_PER_BUFFER;
            } else {
                // Only copy the end samples of the buffer
                size_t numToCpy = FRAMES_PER_BUFFER - (sound->soundBuffer->GetSize() - sound->place)/sizeof(char);
                memcpy(processedFrameSamples[1], (sound->soundBuffer->GetBuffer() + sound->place), numToCpy);
                if (sound->loop) {
                    memcpy(processedFrameSamples[1] + numToCpy*sizeof(char), sound->soundBuffer->GetBuffer(), sizeof(char)*FRAMES_PER_BUFFER-numToCpy);
                    sound->place = sizeof(char) * (FRAMES_PER_BUFFER - numToCpy);
                } else {
                    memset(processedFrameSamples[1] + numToCpy * sizeof(char), 0, sizeof(char)*(FRAMES_PER_BUFFER - numToCpy));
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
        glm::vec3 position = soundScale * entity->position;
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        CheckError("Couldn't set listener position.");
        
        // Set orientation.
        glm::vec4 forward = glm::inverse(entity->GetOrientation()) * glm::vec4(0.f, 0.f, -1.f, 1.f);
        glm::vec4 up = glm::inverse(entity->GetOrientation()) * glm::vec4(0.f, 1.f, 0.f, 1.f);
        ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
        alListenerfv(AL_ORIENTATION, listenerOri);
        CheckError("Couldn't set listener orientation.");
        
        break;
    }
}

int SoundManager::fillOutputBufferCallback(const void* inputBuffer, void* outputBuffer,
                                    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                                    PaStreamCallbackFlags statusFlags, void* userData) {
    // Take samples from processed queue
    (void)inputBuffer;
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    float* out = (float*)outputBuffer;
    for (int i = 0; i < FRAMES_PER_BUFFER; i++)
    {
        *out++ = processedFrameSamples[0][i];
    }
    return paContinue;
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
