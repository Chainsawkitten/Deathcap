#include "Skeleton.hpp"
#include "..\Animation\Skeleton.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <Utility/Log.hpp>
#include <Engine/Hymn.hpp>

using namespace Animation;

Animation::Skeleton::Skeleton() {
}

void Skeleton::Save(std::string path) {
    std::ofstream file(path, std::ios::binary);
    uint32_t size = skeletonBones.size();
    file.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));

    for (unsigned int i = 0; i < size; ++i) {
        file.write(reinterpret_cast<char*>(skeletonBones[i]), sizeof(SkeletonBone));
    }
}

void Skeleton::Load(std::string name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
    std::string path =  (Hymn().GetPath() + "/" + name + ".asset").c_str();

    std::ifstream file(path, std::ios::binary);

    uint32_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    for (unsigned int i = 0; i < size; ++i) {
        SkeletonBone * bone = new SkeletonBone;
        file.read(reinterpret_cast<char*>(bone), sizeof(SkeletonBone));
        skeletonBones.push_back(bone);
    }

    InitSkeleton();
}

void Skeleton::InitSkeleton() {
    // Setup root.
    skeletonBones[0]->parentId = 0;
    skeletonBones[0]->globalTx = skeletonBones[0]->localTx;
    skeletonBones[0]->inversed = glm::inverse(skeletonBones[0]->globalTx);

    // Setup rest of skeleton.
    for (unsigned int i = 1; i < skeletonBones.size(); ++i) {
        skeletonBones[i]->globalTx = skeletonBones[i]->localTx * skeletonBones[skeletonBones[i]->parentId]->globalTx;
        skeletonBones[i]->inversed = glm::inverse(skeletonBones[i]->globalTx);
    }
}
