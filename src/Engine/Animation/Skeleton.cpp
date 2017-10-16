#include "Skeleton.hpp"
#include "..\Animation\Skeleton.hpp"
#include <glm/gtc/matrix_inverse.hpp>

using namespace Animation;

Animation::Skeleton::Skeleton() {
}

void Animation::Skeleton::Save(std::string name) {
    std::ofstream file(name, std::ios::binary);
    uint32_t size = skeletonBones.size();
    file.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));

    for (unsigned int i = 0; i < size; ++i) {
        file.write(reinterpret_cast<char *>(skeletonBones[i]), sizeof(SkeletonBone));
    }
}

void Animation::Skeleton::Load(std::string name) {
    std::ifstream file(name, std::ios::binary);
    uint32_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(uint32_t));

    for (unsigned int i = 0; i < size; ++i) {
        SkeletonBone * bone = new SkeletonBone;
        file.read(reinterpret_cast<char *>(bone), sizeof(SkeletonBone));
        skeletonBones.push_back(bone);
    }

    InitSkeleton();
}

void Animation::Skeleton::InitSkeleton() {
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
