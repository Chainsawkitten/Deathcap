#include "SoundBuffer.hpp"

#include "../Manager/SoundManager.hpp"
#include "SoundFile.hpp"
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include "VorbisFile.hpp"

using namespace Audio;

SoundBuffer::SoundBuffer() {
    
}

SoundBuffer::SoundBuffer(SoundFile* soundFile) {
    Load(soundFile);
}

SoundBuffer::~SoundBuffer() {
    alDeleteBuffers(1, &buffer);
}

ALuint SoundBuffer::GetBuffer() const {
    return buffer;
}

Json::Value SoundBuffer::Save() const {
    Json::Value sound;
    sound["name"] = name;
    return sound;
}

void SoundBuffer::Load(const std::string& name) {
    this->name = name;
    SoundFile* soundFile = new VorbisFile((Hymn().GetPath() + FileSystem::DELIMITER + "Sounds" + FileSystem::DELIMITER + name + ".ogg").c_str());
    Load(soundFile);
    delete soundFile;
}

void SoundBuffer::Load(SoundFile* soundFile) {
    // Create audio buffer.
    alGetError();
    alGenBuffers((ALuint)1, &buffer);
    SoundManager::CheckError("Couldn't create buffers.");
    
    // Set the buffer data.
    alBufferData(buffer, soundFile->GetFormat(), soundFile->GetData(), soundFile->GetSize(), soundFile->GetSampleRate());
    SoundManager::CheckError("Couldn't set buffer data.");
}
