#include "AnimationController.hpp"
#include "../Animation/AnimationController.hpp"
#include "../Animation/Skeleton.hpp"
#include "../Animation/AnimationClip.hpp"

using namespace Component;

AnimationController::AnimationController() {
    for (unsigned int i = 0; i < 30; ++i) {
        bones.push_back(glm::mat4(1.0f));
    }
}

Json::Value AnimationController::Save() const {
    Json::Value component;
    component["animationController"] = controller->name;
    component["skeleton"] = skeleton->name;
    return component;
}

void Component::AnimationController::UpdateAnimation(float deltaTime) {
//    if (controller->activeAction1 != nullptr) {
//        Animation::AnimationClip* clip = controller->animationClips[controller->activeAction1->name];
//        Animation::AnimationClip::Animation* animation = clip->animation;
//
//        for (unsigned int bone = 1; bone < animation->numBones; ++bone) {
//            if (bone > skeleton->skeletonBones.size())
//                break;
//
//            skeleton->skeletonBones[bone]->globalTx = skeleton->skeletonBones[skeleton->skeletonBones[bone]->parentId]->globalTx * glm::mat4(1.0f); //animation.
//            bones[bone] = skeleton->skeletonBones[bone]->globalTx * skeleton->skeletonBones[bone]->inversed;
//        }
//    }
}
