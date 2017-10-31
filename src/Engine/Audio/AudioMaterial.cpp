#include "AudioMaterial.hpp"

using namespace Audio;

AudioMaterial::AudioMaterial() {
    
}

AudioMaterial::~AudioMaterial() {

}

Json::Value AudioMaterial::Save() const {
    Json::Value audioMaterial;
    audioMaterial["name"] = name;
    return audioMaterial;
}

void AudioMaterial::Load(const std::string& name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
}
