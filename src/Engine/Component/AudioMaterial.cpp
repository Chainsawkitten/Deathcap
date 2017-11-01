#include "AudioMaterial.hpp"

using namespace Component;

AudioMaterial::AudioMaterial() {
    
}

Json::Value AudioMaterial::Save() const {
    Json::Value component;
    component["placeholderValue"] = "";
    return component;
}
