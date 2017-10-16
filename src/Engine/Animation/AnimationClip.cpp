#include "AnimationClip.hpp"
#include <fstream>

void Animation::AnimationClip::Load(const std::string & name) {
    std::ifstream file(name);
    
    animation = new Animation;
    animation->Load(&file);
    file.close();
}
