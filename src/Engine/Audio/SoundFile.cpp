#include "SoundFile.hpp"

using namespace Audio;

SoundFile::~SoundFile() {

}

Json::Value SoundFile::Save() const {
    Json::Value sound;
    sound["name"] = name;
    return sound;
}

void SoundFile::Load(const std::string& name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
    std::string filename = Hymn().GetPath() + "/" + name + ".ogg";
    Load(filename.c_str());
}
