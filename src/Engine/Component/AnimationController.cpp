#include "AnimationController.hpp"
#include "../Animation/Skeleton.hpp"
#include "../Animation/AnimationClip.hpp"
#include <Utility/Log.hpp>
#include "glm/gtc/quaternion.hpp"

using namespace Component;

AnimationController::AnimationController() {
    // Push back identity matrcies.
    for (unsigned int i = 0; i < 50; ++i) {
        bones.push_back(glm::mat4(1.0f));
    }
}

Json::Value AnimationController::Save() const {
    Json::Value component;

    if (controller != nullptr)
        component["animationController"] = controller->path + controller->name;

    if (skeleton != nullptr)
        component["skeleton"] = skeleton->path + skeleton->name;

    return component;
}

void Component::AnimationController::UpdateAnimation(float deltaTime) {
    if (skeleton == nullptr || controller == nullptr)
        return;

    if (bones.size() != skeleton->skeletonBones.size()) {
        bones.clear();
        bones.shrink_to_fit();
        bones.resize(skeleton->skeletonBones.size());
    }

    // Hardcoded node system at this moment.
    /// @todo Remove hardcoded node system.
    if (activeAction1 == nullptr) 
        activeAction1 = dynamic_cast<Animation::AnimationController::AnimationAction*>(controller->animationNodes[0]);

    Animate(deltaTime, activeAction1);
//    Animate(deltaTime, activeAction2);
    Interpolate(deltaTime);
}

void AnimationController::SetBool(std::string name, bool value) {
    return void();
}

void Component::AnimationController::SetFloat(std::string name, float value) {
    return;
}

void Component::AnimationController::Animate(float deltaTime, Animation::AnimationController::AnimationAction* action) {
    Animation::AnimationClip::Animation* anim = action->animationClip->animation;
    unsigned int size = skeleton->skeletonBones.size() > anim->numBones ? anim->numBones : skeleton->skeletonBones.size();

    anim->currentFrame += deltaTime * 1.0f;
    if (anim->currentFrame > 15.0f) {
        anim->currentFrame = 0;

        for (unsigned int i = 0; i < size; ++i)
            anim->bones[i].currentKeyIndex = 0;
    }

    skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx;
    bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;

    for (unsigned int i = 1; i < size; ++i) {
        Animation::AnimationClip::Bone* bone = &anim->bones[i];

        if ((float)bone->rotationKeys[bone->currentKeyIndex + 1] > anim->currentFrame)
            ++bone->currentKeyIndex;

        float interpolation = (float)bone->rotationKeys[bone->currentKeyIndex + 1] - anim->currentFrame;
        interpolation /= bone->rotationKeys[bone->currentKeyIndex + 1];
        interpolation = 1 - interpolation;

        // Clamp interpolation.
        if (interpolation > 0.999f)
            interpolation = 0.999f;
        else if (interpolation < 0.001f)
            interpolation = 0.001f;

        // Convert to quaternion to interpolate animation then back to matrix.
        glm::quat rotation1 = glm::quat_cast(bone->rotations[bone->currentKeyIndex]);
        glm::quat rotation2 = glm::quat_cast(bone->rotations[bone->currentKeyIndex + 1]);
        glm::quat finalRot = glm::lerp(rotation1, rotation2, interpolation);

        glm::mat4 matrixRot = glm::mat4(finalRot);
        matrixRot[3][3] = 1.0f;

        skeleton->skeletonBones[i]->globalTx = skeleton->skeletonBones[skeleton->skeletonBones[i]->parentId]->globalTx * skeleton->skeletonBones[i]->localTx * matrixRot;
        bones[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
    }
}

void AnimationController::Interpolate(float deltaTime) {

}

bool AnimationController::GetBool(std::string name) {
    return true;
}

float AnimationController::GetFloat(std::string name) {
    return 1.0f;
}
