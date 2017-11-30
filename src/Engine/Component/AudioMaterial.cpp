#include "AudioMaterial.hpp"
#include "../Audio/AudioMaterial.hpp"

using namespace Component;

AudioMaterial::AudioMaterial() {
    
}

Json::Value AudioMaterial::Save() const {
    Json::Value component;
    component["audio material"] = material->path + material->name;
    return component;
}
