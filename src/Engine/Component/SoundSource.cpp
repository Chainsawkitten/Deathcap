#include "SoundSource.hpp"

#include "../Entity/Entity.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "../Audio/SoundFile.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "../Manager/Managers.hpp"
#include "../Manager/ResourceManager.hpp"

using namespace Component;

SoundSource::SoundSource() {
    soundBuffer = new Audio::SoundBuffer();
}

SoundSource::~SoundSource() {
    Audio::SoundFile* soundFile = soundBuffer->GetSoundFile();
    if (soundFile)
        Managers().resourceManager->FreeSound(soundFile);
    delete soundBuffer;
    if (renderers)
        delete renderers;
}

Json::Value SoundSource::Save() const {
    Json::Value component;
    
    Audio::SoundFile* soundFile = soundBuffer->GetSoundFile();
    if (soundFile) {
        component["sound"] = soundFile->path + soundFile->name;
    }
    
    component["volume"] = volume;
    component["loop"] = loop;
    return component;
}

void SoundSource::Play() {
    shouldPlay = true;
}

void SoundSource::Pause() {
    shouldPause = true;
}

void SoundSource::Stop() {
    shouldStop = true;
}
