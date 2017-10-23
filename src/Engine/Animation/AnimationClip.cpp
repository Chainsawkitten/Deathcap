#include "AnimationClip.hpp"
#include <fstream>
#include "../Hymn.hpp"
#include <Utility/Log.hpp>

using namespace Animation;

void AnimationClip::Load(const std::string & name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
    std::string path = (Hymn().GetPath() + "/" + name + ".asset").c_str();

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        Log() << "Failed to open file: " << path << "\n";
        file.close();
        return;
    }
    
    animation = new AnimationClip::Animation;
    animation->Load(&file);
    file.close();
}
