#include "SoundFile.hpp"

#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include <fstream>
#include <json\json.h>

using namespace Audio;

SoundFile::~SoundFile() {

}

bool SoundFile::GetCached() const {
    return (buffer != nullptr);
}

void SoundFile::Save() const {
    Json::Value node;
    node["name"] = name;
    node["cache"] = (buffer != nullptr);

    // Save properties to meta file.
    std::string filename = Hymn().GetPath() + "/" + path + name + ".json";
    std::ofstream file(filename);
    file << node;
    file.close();
}

void SoundFile::Load(const std::string& name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
  
    std::string filename = Hymn().GetPath() + "/" + name;
    Load(std::string(filename + ".ogg").c_str());

    // Get properties from meta file.
    Json::Value node;
    if (!FileSystem::FileExists(std::string(filename + ".json").c_str()))
        Save();

    std::ifstream file(filename + ".json");
    file >> node;
    file.close();

    bool cache = node.get("cache", false).asBool();
    Cache(cache);
}
