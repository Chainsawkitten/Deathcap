#include "AudioMaterial.hpp"
#include "Hymn.hpp"
#include "Util/FileSystem.hpp"
#include <fstream>

using namespace Audio;

AudioMaterial::AudioMaterial() {
    
}

AudioMaterial::~AudioMaterial() {

}

void AudioMaterial::Save() const {
    Json::Value audioMaterial;
    audioMaterial["name"] = name;
    audioMaterial["lowFreqAbsorption"] = lowFreqAbsorption;
    audioMaterial["midFreqAbsorption"] = midFreqAbsorption;
    audioMaterial["highFreqAbsorption"] = highFreqAbsorption;
    audioMaterial["scattering"] = scattering;
    audioMaterial["lowFreqTransmission"] = lowFreqTransmission;
    audioMaterial["midFreqTransmission"] = midFreqTransmission;
    audioMaterial["highFreqTransmission"] = highFreqTransmission;

    // Save properties to meta file.
    std::string filename = Hymn().GetPath() + "/" + path + name + ".json";
    std::ofstream file(filename);
    file << audioMaterial;
    file.close();
}

void AudioMaterial::Load(const std::string& name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);

    std::string filename = Hymn().GetPath() + "/" + name;

    // Get properties from meta file.
    Json::Value node;
    if (!FileSystem::FileExists(std::string(filename + ".json").c_str())) {
        Save();
    }
    std::ifstream file(filename + ".json");
    file >> node;
    file.close();

    lowFreqAbsorption = node.get("lowFreqAbsorption", 0.5f).asFloat();
    midFreqAbsorption = node.get("midFreqAbsorption", 0.5f).asFloat();
    highFreqAbsorption = node.get("highFreqAbsorption", 0.5f).asFloat();
    scattering = node.get("scattering", 0.5f).asFloat();
    lowFreqTransmission = node.get("lowFreqTransmission", 0.5f).asFloat();
    midFreqTransmission = node.get("midFreqTransmission", 0.5f).asFloat();
    highFreqTransmission = node.get("highFreqTransmission", 0.5f).asFloat();
}
