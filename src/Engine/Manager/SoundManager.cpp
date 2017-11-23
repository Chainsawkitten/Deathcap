#include "SoundManager.hpp"
#include <phonon.h>
#include <Utility/Log.hpp>
#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/AudioMaterial.hpp"
#include "../Component/Listener.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/SoundSource.hpp"
#include "../Audio/SoundFile.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "../Audio/SoundStreamer.hpp"
#include "../Audio/AudioMaterial.hpp"
#include <Video/Geometry/Geometry3D.hpp>
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include <portaudio.h>
#include <cstdint>
#include <cstring>
#include <algorithm>

SoundManager::SoundManager() {
    PaError err;

    // Initialize PortAudio
    err = Pa_Initialize();
    CheckError(err);

    PaStreamParameters outputParams;

    outputParams.device = Pa_GetDefaultOutputDevice();
    if (outputParams.device >= 0) {
        outputParams.channelCount = 2;
        outputParams.sampleFormat = paFloat32;
        outputParams.hostApiSpecificStreamInfo = NULL;
        outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultHighOutputLatency;
    }

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

    err = Pa_StartStream(stream);
    CheckError(err);
}


SoundManager::~SoundManager() {
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

void SoundManager::Update(float deltaTime) {

    // Number of samples to process dependant on deltaTime
    /*unsigned int frameSamples = int(SAMPLE_RATE * deltaTime);
    if (frameSamples > CHUNK_SIZE) {
        //Log() << "SoundManager::Update: Frame drop!\n";
        frameSamples = CHUNK_SIZE;
    }
    targetSample += frameSamples;

    // Update sound.
    while (currentSample < targetSample) {
        // Process Samples.
        if (processedSamples == 0) {
            ProcessSamples();
            processedSamples = CHUNK_SIZE;
        }

        // Play samples.
        unsigned int sampleCount = std::min(targetSample - currentSample, processedSamples);
        unsigned int index = (CHUNK_SIZE - processedSamples) * 2;
        Pa_WriteStream(stream, &processedBuffer[index], sampleCount);
        processedSamples -= sampleCount;
        currentSample += sampleCount;
    }*/
}

void SoundManager::ProcessSamples() {

    // Set player transform
    std::vector<Component::Listener*> listeners = GetListeners();
    assert(listeners[0] != nullptr);
    Entity* player = listeners[0]->entity;
    glm::vec3 glmPos = player->GetWorldPosition();
    glm::quat orientation = player->GetWorldOrientation();
    glm::vec3 glmDir = orientation * glm::vec3(0, 0, -1);
    glm::vec3 glmUp = orientation * glm::vec3(0, 1, 0);
    IPLVector3 pos = { glmPos.x, glmPos.y, glmPos.z };
    IPLVector3 dir = { glmDir.x, glmDir.y, glmDir.z };
    IPLVector3 up = { glmUp.x, glmUp.y, glmUp.z };

    sAudio.SetPlayer(pos, dir, up);

    std::size_t size = soundSources.GetAll().size();
    std::vector<Audio::SoundBuffer*> soundBuffers;
    soundBuffers.resize(size);
    std::vector<float*> buffers;
    buffers.resize(size);
    std::vector<IPLVector3> positions;
    positions.resize(size);
    std::vector<float> radii;
    radii.resize(size);

    // Update sound sources.
    for (std::size_t i = 0; i < size; ++i) {

        Component::SoundSource* sound = soundSources.GetAll()[i];
        Audio::SoundBuffer* soundBuffer = sound->soundBuffer;
        Audio::SoundFile* soundFile = soundBuffer->GetSoundFile();

        // Check if sound should play and is a valid resource.
        //if (sound->shouldPlay && soundFile) { //TMPTODO
        if (soundFile) {
            soundBuffers[i] = soundBuffer;

            // Get samples from streamed buffer.
            int samples;
            buffers[i] = soundBuffer->GetChunkData(samples);

            // Volume.
            for (int m = 0; m < samples; ++m)
                buffers[i][m] *= sound->volume;

            glm::vec3 position = sound->entity->GetWorldPosition();
            positions[i] = IPLVector3{ position.x, position.y, position.z };
            radii[i] = 5.f;

            // If end of file, check if sound repeat.
            if (samples == 0)
                if (sound->loop)
                    soundBuffer->Restart();
                else
                    sound->shouldStop = true;
        }

        // Pause it.
        if (sound->shouldPause) {
            sound->shouldPlay = false;
        }

        // Stop it.
        if (sound->shouldStop) {
            sound->shouldPlay = false;
        }

    }

    // Process sound.
    if (!soundBuffers.empty())
        sAudio.Process(buffers, positions, radii, processedBuffer);

    // Consume used chunk and produce new chunk.
    for (Audio::SoundBuffer* soundBuffer : soundBuffers) {
        soundBuffer->ConsumeChunk();
        soundBuffer->ProduceChunk();
    }

}


Component::SoundSource* SoundManager::CreateSoundSource() {
    Component::SoundSource* soundSource = soundSources.Create();
    return soundSource;
}

Component::SoundSource* SoundManager::CreateSoundSource(const Json::Value& node) {
    Component::SoundSource* soundSource = soundSources.Create();

    // Load values from Json node.
    std::string name = node.get("sound", "").asString();
    if (!name.empty())
        soundSource->soundBuffer->SetSoundFile(Managers().resourceManager->CreateSound(name));

    soundSource->volume = node.get("volume", 1.f).asFloat();
    soundSource->loop = node.get("loop", false).asBool();

    return soundSource;
}

const std::vector<Component::SoundSource*>& SoundManager::GetSoundSources() const {
    return soundSources.GetAll();
}

Component::Listener* SoundManager::CreateListener() {
    Component::Listener* listener = listeners.Create();
    return listener;
}

Component::Listener* SoundManager::CreateListener(const Json::Value& node) {
    Component::Listener* listener = listeners.Create();
    return listener;
}

const std::vector<Component::Listener*>& SoundManager::GetListeners() const {
    return listeners.GetAll();
}

Component::AudioMaterial* SoundManager::CreateAudioMaterial() {
    Component::AudioMaterial* audioMaterial = audioMaterials.Create();
    return audioMaterial;
}

Component::AudioMaterial* SoundManager::CreateAudioMaterial(const Json::Value& node) {
    Component::AudioMaterial* audioMaterial = audioMaterials.Create();

    // Load values from Json node.
    std::string name = node.get("audio material", "").asString();
    if (!name.empty())
        audioMaterial->material = Managers().resourceManager->CreateAudioMaterial(name);

    return audioMaterial;
}

const std::vector<Component::AudioMaterial*>& SoundManager::GetAudioMaterials() const {
    return audioMaterials.GetAll();
}

void SoundManager::CreateAudioEnvironment() {

    // Create new scene if no scene can be loaded
    //if (NOSUCHFILE)
    {
        // Temporary list of all audio materials in use
        std::vector<Audio::AudioMaterial*> audioMatRes;

        int numMaterials = 0;
        // Get all material resources in use
        for (const Component::AudioMaterial* audioMatComp : GetAudioMaterials()) {

            std::vector<Audio::AudioMaterial*>::iterator it;
            it = std::find(audioMatRes.begin(), audioMatRes.end(), audioMatComp->material);
            // Add the resource if it's not already in the list
            if (it == audioMatRes.end()) {
                audioMatRes.push_back(audioMatComp->material);
                numMaterials++;
            }
        }

        // Create Scene
        sAudio.CreateScene(audioMatRes.size());

        for (int i = 0; i < audioMatRes.size(); i++) {
            IPLMaterial iplmat;
            iplmat.highFreqAbsorption = audioMatRes[i]->highFreqAbsorption;
            iplmat.midFreqAbsorption = audioMatRes[i]->midFreqAbsorption;
            iplmat.lowFreqAbsorption = audioMatRes[i]->lowFreqAbsorption;
            iplmat.highFreqTransmission = audioMatRes[i]->highFreqTransmission;
            iplmat.midFreqTransmission = audioMatRes[i]->midFreqTransmission;
            iplmat.lowFreqTransmission = audioMatRes[i]->lowFreqTransmission;
            iplmat.scattering = audioMatRes[i]->scattering;

            sAudio.SetSceneMaterial(i, iplmat);
        }

        // Create mesh.
        for (const Component::AudioMaterial* audioMatComp : GetAudioMaterials()) {
            Entity* entity = audioMatComp->entity;
            Component::Mesh* mesh = entity->GetComponent<Component::Mesh>();
            if (mesh && mesh->geometry) {
                const std::vector<glm::vec3>& meshVertices = mesh->geometry->GetVertexPositionData();
                const std::vector<uint32_t>& meshIndices = mesh->geometry->GetVertexIndexData();

                // Create ipl mesh if vertex data is valid.
                if (meshVertices.size() > 0 && meshIndices.size() > 0) {
                    const glm::mat4 modelMatrix = entity->GetModelMatrix();
                    std::vector<IPLVector3> iplVertices;
                    std::vector<IPLTriangle> iplIndices;

                    // Convert and transform vertices.
                    iplVertices.resize(meshVertices.size());
                    for (std::size_t i = 0; i < meshVertices.size(); ++i) {
                        const glm::vec4 transformedVector = modelMatrix * glm::vec4(meshVertices[i], 1.f);
                        iplVertices[i] = IPLVector3{ transformedVector.x, transformedVector.y, transformedVector.z };
                    }

                    // Convert indices.
                    iplIndices.resize(meshIndices.size());
                    for (std::size_t i = 0; i < meshIndices.size(); ++i) {
                        iplIndices[i] = IPLTriangle{ (IPLint32)meshIndices[i] };
                    }

                    // Find material index and create ipl mesh.
                    for (int i = 0; i < audioMatRes.size(); i++) {
                        if (audioMatRes[i] == audioMatComp->material) {
                            sAudio.CreateStaticMesh(iplVertices, iplIndices, i);
                            break;
                        }
                    }
                }
            }
            
        }

        sAudio.FinalizeScene(NULL);

        // Save scene
        //SteamAudioInterface::SaveData saveData = sAudio.SaveFinalizedScene();
    }
    //else {

    //}

    // Create Environment
    sAudio.CreateEnvironment();
}

void SoundManager::ClearKilledComponents() {
    soundSources.ClearKilled();
    listeners.ClearKilled();
}

void SoundManager::Load(Audio::SoundStreamer::DataHandle& dataHandle) {
    soundStreamer.Load(dataHandle);
}

void SoundManager::Flush(std::queue<Audio::SoundStreamer::DataHandle>& queue) {
    soundStreamer.Flush(queue);
}