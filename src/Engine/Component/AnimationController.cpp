#include "AnimationController.hpp"

#include "../Animation/Skeleton.hpp"
#include "../Animation/AnimationClip.hpp"
#include <Utility/Log.hpp>
#include "glm/gtc/quaternion.hpp"
#include <cstring>

using namespace Component;

AnimationController::AnimationController() {
    // Push back identity matrices.
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

    if (skeleton != nullptr && bones.size() != skeleton->skeletonBones.size()) {
        bones.clear();
        bones.resize(skeleton->skeletonBones.size());
        bonesToInterpolate1.clear();
        bonesToInterpolate1.resize(skeleton->skeletonBones.size());
        bonesToInterpolate2.clear();
        bonesToInterpolate2.resize(skeleton->skeletonBones.size());
    }

    // Select the first node in the animation controller.
    // This will work as the entry point for the animation.
    if (activeAction1 == nullptr) {
        if (controller->animationNodes.size() > 0) {
            Animation::AnimationController::AnimationAction* tmpAction = dynamic_cast<Animation::AnimationController::AnimationAction*>(controller->animationNodes[0]);
            if (tmpAction != nullptr)
                activeAction1 = tmpAction;
            else
                return;
        } else
            return;
    }

    if (activeTransition == nullptr) {
        for (uint32_t i = 0; i < activeAction1->numOutputSlots; ++i) {
            Animation::AnimationController::AnimationTransition* tmpTransition = dynamic_cast<Animation::AnimationController::AnimationTransition*>(controller->animationNodes[activeAction1->outputIndex[i]]);
            if (tmpTransition != nullptr) {
                if ((controller->boolMap.empty() || tmpTransition->isStatic) && tmpTransition->numOutputSlots > 0 && tmpTransition->isStatic) {
                    Animation::AnimationController::AnimationAction* tmpAction = dynamic_cast<Animation::AnimationController::AnimationAction*>(controller->animationNodes[tmpTransition->outputIndex[0]]);
                    if (tmpAction != nullptr) {
                        activeTransition = tmpTransition;
                        activeTransition->transitionProcess = 0.0f;
                        activeAction2 = tmpAction;
                        isBlending = true;
                    }
                } else if (!tmpTransition->isStatic && !controller->boolMap.empty() && controller->boolMap[tmpTransition->transitionBoolIndex]) {
                    // If the bool in the boolMap is true then set this to the activeTransition.
                    activeTransition = tmpTransition;
                    activeTransition->transitionProcess = 0.0f;
                    break;
                }
            }
        }
    }

    if (activeTransition == nullptr)
        Animate(deltaTime, activeAction1);
    else {
        activeTransition->transitionProcess += deltaTime;
        if (activeTransition->transitionProcess > activeTransition->transitionTime) {
            activeTransition->transitionProcess = 0.0f;
            activeTransition = nullptr;

            // Set action 1 to action 2.
            activeAction1 = activeAction2;
            activeAction2 = nullptr;

            isBlending = false;

            Animate(deltaTime, activeAction1);
        } else {
            Animate(deltaTime, activeAction1, 1);
            Animate(deltaTime, activeAction2, 2);
            Interpolate(deltaTime);
        }
    }

    Animate(deltaTime, activeAction1);
}

void AnimationController::Animate(float deltaTime, Animation::AnimationController::AnimationAction* action, unsigned int skeletonId) {
    Animation::AnimationClip::Animation* anim = action->animationClip->animation;
    std::size_t size = skeleton->skeletonBones.size() > anim->numBones ? anim->numBones : skeleton->skeletonBones.size();

    anim->currentFrame += deltaTime * 30.0f;
    if (anim->currentFrame > 15.0f) {
        anim->currentFrame = 0;

        for (unsigned int i = 0; i < size; ++i)
            anim->bones[i].currentKeyIndex = 0;
    }

    skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx;
    bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;

    for (std::size_t i = 1; i < size; ++i) {
        Animation::AnimationClip::Bone* bone = &anim->bones[i];

        // Loop if the animation is very fast.
        while ((float)bone->rotationKeys[bone->currentKeyIndex + 1] < anim->currentFrame)
            ++bone->currentKeyIndex;

        float interpolation = (anim->currentFrame - (float)bone->rotationKeys[bone->currentKeyIndex]) / ((float)bone->rotationKeys[bone->currentKeyIndex + 1] - (float)bone->rotationKeys[bone->currentKeyIndex]);

        // Clamp interpolation.
        if (interpolation > 0.999f)
            interpolation = 0.999f;
        else if (interpolation < 0.001f)
            interpolation = 0.001f;

        // Convert to quaternion to interpolate animation then back to matrix.
        glm::quat rotation1 = glm::quat_cast(bone->rotations[bone->currentKeyIndex]);
        glm::quat rotation2 = glm::quat_cast(bone->rotations[bone->currentKeyIndex + 1]);
        glm::quat finalRot = glm::slerp(rotation1, rotation2, interpolation);

        glm::mat4 matrixRot = glm::mat4(finalRot);

        if (skeleton->skeletonBones[i]->parentId == -1)
            continue;
        
        skeleton->skeletonBones[i]->globalTx = skeleton->skeletonBones[skeleton->skeletonBones[i]->parentId]->globalTx * skeleton->skeletonBones[i]->localTx * matrixRot;
        
        // If is interpolating.
        if (isBlending && skeletonId == 1)
            bonesToInterpolate1[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
        else if (isBlending && skeletonId == 2)
            bonesToInterpolate2[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
        else
            bones[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
    }
}

void AnimationController::Interpolate(float deltaTime) {
    uint32_t size = activeAction1->animationClip->animation->numBones > activeAction2->animationClip->animation->numBones ? activeAction2->animationClip->animation->numBones : activeAction1->animationClip->animation->numBones;

    if (activeAction1->animationClip->animation->numBones != bonesToInterpolate1.size()) {
        bonesToInterpolate1.clear();
        bonesToInterpolate1.resize(activeAction1->animationClip->animation->numBones);
    }

    if (activeAction2->animationClip->animation->numBones != bonesToInterpolate2.size()) {
        bonesToInterpolate2.clear();
        bonesToInterpolate2.resize(activeAction2->animationClip->animation->numBones);
    }

    for (uint32_t i = 0; i < size; ++i) {
        float interpolation = activeTransition->transitionProcess / activeTransition->transitionTime;

        // Clamp interpolation.
        if (interpolation > 0.999f)
            interpolation = 0.999f;
        else if (interpolation < 0.001f)
            interpolation = 0.001f;

        // Convert to quaternion to interpolate animation then back to matrix.
        glm::quat rotation1 = glm::quat_cast(bonesToInterpolate1[i]);
        glm::quat rotation2 = glm::quat_cast(bonesToInterpolate2[i]);
        glm::quat finalRot = glm::lerp(rotation1, rotation2, interpolation);

        glm::mat4 matrixRot = glm::mat4(finalRot);

        bones[i] = matrixRot;
    }
}

void AnimationController::SetBool(const std::string& name, bool value) {
    for (std::size_t i = 0; i < controller->boolMap.size(); ++i) {
        if (strcmp(name.c_str(), controller->boolMap[i]->name) == 0) {
            controller->boolMap[i]->value = value;
            return;
        }
    }
}

void AnimationController::SetFloat(const std::string& name, float value) {
    for (std::size_t i = 0; i < controller->floatMap.size(); ++i) {
        if (strcmp(name.c_str(), controller->floatMap[i]->name) == 0) {
            controller->floatMap[i]->value = value;
            return;
        }
    }
}

bool AnimationController::GetBool(const std::string& name) {
    for (std::size_t i = 0; i < controller->boolMap.size(); ++i) {
        if (strcmp(name.c_str(), controller->boolMap[i]->name) == 0) {
            return controller->boolMap[i]->value;
        }
    }
}

float AnimationController::GetFloat(const std::string& name) {
    for (std::size_t i = 0; i < controller->floatMap.size(); ++i) {
        if (strcmp(name.c_str(), controller->floatMap[i]->name) == 0) {
            return controller->floatMap[i]->value;
        }
    }
}
