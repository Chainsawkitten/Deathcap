#include "AnimationController.hpp"

#include "../Animation/Skeleton.hpp"
#include "../Animation/AnimationClip.hpp"
#include <Utility/Log.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cstring>

using namespace Component;

AnimationController::AnimationController() {
    // Push back identity matrices.
    for (unsigned int i = 0; i < 100; ++i) {
        bones.push_back(glm::mat4(1.0f));
    }
}

Json::Value AnimationController::Save() const {
    Json::Value component;

    if (controller)
        component["animationController"] = controller->path + controller->name;

    if (skeleton)
        component["skeleton"] = skeleton->path + skeleton->name;

    return component;
}

void Component::AnimationController::UpdateAnimation(float deltaTime) {
    if (!skeleton || !controller)
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

    if (!activeTransition && !activeAction1->repeat) {
        for (uint32_t i = 0; i < activeAction1->numOutputSlots; ++i) {
            Animation::AnimationController::AnimationTransition* tmpTransition = dynamic_cast<Animation::AnimationController::AnimationTransition*>(controller->animationNodes[activeAction1->outputIndex[i]]);
            if (tmpTransition) {
                if ((controller->boolMap.empty() || tmpTransition->isStatic) && tmpTransition->numOutputSlots > 0 && tmpTransition->isStatic) {
                    Animation::AnimationController::AnimationAction* tmpAction = dynamic_cast<Animation::AnimationController::AnimationAction*>(controller->animationNodes[tmpTransition->outputIndex[0]]);
                    if (tmpAction && (activeAction1->animationClip->animation->currentFrame / activeAction1->animationClip->animation->length) > 1 - tmpTransition->transitionTime) {
                        activeTransition = tmpTransition;
                        activeTransition->transitionProcess = 0.f;
                        activeAction2 = tmpAction;
                        isBlending = true;
                    }
                } else if (!tmpTransition->isStatic && !controller->boolMap.empty() && controller->boolMap[tmpTransition->transitionBoolIndex]) {
                    // If the bool in the boolMap is true then set this to the activeTransition.
                    activeTransition = tmpTransition;
                    activeTransition->transitionProcess = 0.f;
                    break;
                }
            }
        }
    }

    if (!isBlending)
        Animate(deltaTime, activeAction1);
    else {
        activeTransition->transitionProcess += (deltaTime * 24.f * activeAction1->playbackModifier) / activeAction1->animationClip->animation->length;
        if (activeTransition->transitionProcess > activeTransition->transitionTime) {
            activeTransition->transitionProcess = 0.f;
            activeTransition = nullptr;

            // Set action 1 to action 2.
            activeAction1 = activeAction2;
            activeAction2 = nullptr;

            isBlending = false;

            Animate(deltaTime, activeAction1);
        } else {
            Animate(deltaTime, activeAction1, 1);
            Animate(deltaTime, activeAction2, 2);
            Blend(deltaTime);
        }
    }
}

#include <iostream>

void AnimationController::Animate(float deltaTime, Animation::AnimationController::AnimationAction* action, unsigned int skeletonId) {
    Animation::AnimationClip::Animation* anim = action->animationClip->animation;
    std::size_t size = skeleton->skeletonBones.size() > anim->numBones ? anim->numBones : skeleton->skeletonBones.size();

    anim->currentFrame += deltaTime * 24.0f * activeAction1->playbackModifier;
    if (anim->currentFrame > anim->length) {
        anim->currentFrame = 0;
        anim->currentRootKeyIndex = 0;

        for (unsigned int i = 0; i < size; ++i)
            anim->bones[i].currentKeyIndex = 0;
    }

    // Animate root bone positions.
    // Loop if the animation is very fast.
    while ((float)anim->rootPositionKeys[anim->currentRootKeyIndex + 1] < anim->currentFrame)
        ++anim->currentRootKeyIndex;

    // Interpolation of position keys.
    float interpolation = (anim->currentFrame - (float)anim->rootPositionKeys[anim->currentRootKeyIndex]) / ((float)anim->rootPositionKeys[anim->currentRootKeyIndex + 1] - (float)anim->rootPositionKeys[anim->currentRootKeyIndex]);
    interpolation *= 2.f;
    interpolation -= 1.f;
    interpolation = glm::sin(interpolation * (glm::pi<float>() / 2.f));
    interpolation += 1.f;
    interpolation /= 2.f;
 
    glm::vec3 pos1 = anim->rootPositions[anim->currentRootKeyIndex] * (1.f - interpolation);
    glm::vec3 pos2 = anim->rootPositions[anim->currentRootKeyIndex + 1] * interpolation;

    if (skeletonId == 1) {
        position1 = pos1 + pos2;
        skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx;
        bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;
    } else if (skeletonId == 2) {
        position2 = pos1 + pos2;
        skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx;
        bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;
    } else {
        glm::vec3 finalPos = pos1 - pos2;
        glm::mat4 matrixPos1 = glm::mat4(1.f);
        matrixPos1 = glm::translate(matrixPos1, anim->rootPositions[anim->currentRootKeyIndex]);

        glm::mat4 matrixPos2 = glm::mat4(1.f);
        matrixPos2 = glm::translate(matrixPos2, pos2);

        std::cout << anim->currentRootKeyIndex << "\n";

        skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx * matrixPos1;
        bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;
    }

    // If is interpolating.
    if (isBlending && skeletonId == 1)
        bonesToInterpolate1[0] = glm::mat4(1.f);
    else if (isBlending && skeletonId == 2)
        bonesToInterpolate2[0] = glm::mat4(1.f);

    for (std::size_t i = 1; i < size; ++i) {
        Animation::AnimationClip::Bone* bone = &anim->bones[i];

        // Loop if the animation is very fast.
        while ((float)bone->rotationKeys[bone->currentKeyIndex + 1] < anim->currentFrame)
            ++bone->currentKeyIndex;

        float interpolation = (anim->currentFrame - (float)bone->rotationKeys[bone->currentKeyIndex]) / ((float)bone->rotationKeys[bone->currentKeyIndex + 1] - (float)bone->rotationKeys[bone->currentKeyIndex]);
        interpolation *= 2.f;
        interpolation -= 1.f;
        interpolation = glm::sin(interpolation * (glm::pi<float>() / 2.f));
        interpolation += 1.f;
        interpolation /= 2.f;

        // Clamp interpolation.
        if (interpolation > 0.999f)
            interpolation = 0.999f;
        else if (interpolation < 0.001f)
            interpolation = 0.001f;

        // Convert to quaternion to interpolate animation then back to matrix.
        glm::mat4 finalRot = glm::mat4(glm::slerp(bone->rotations[bone->currentKeyIndex], bone->rotations[bone->currentKeyIndex + 1], interpolation));
        if (skeleton->skeletonBones[i]->parentId == -1)
            continue;

        // If is interpolating.
        if (isBlending && skeletonId == 1)
            bonesToInterpolate1[i] = finalRot;
        else if (isBlending && skeletonId == 2)
            bonesToInterpolate2[i] = finalRot;
        else {
            skeleton->skeletonBones[i]->globalTx = skeleton->skeletonBones[skeleton->skeletonBones[i]->parentId]->globalTx * skeleton->skeletonBones[i]->localTx * finalRot;
            bones[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
        }
    }
}

void AnimationController::Blend(float deltaTime) {
    uint32_t size = activeAction1->animationClip->animation->numBones > activeAction2->animationClip->animation->numBones ? activeAction2->animationClip->animation->numBones : activeAction1->animationClip->animation->numBones;

    if (activeAction1->animationClip->animation->numBones != bonesToInterpolate1.size()) {
        bonesToInterpolate1.clear();
        bonesToInterpolate1.resize(activeAction1->animationClip->animation->numBones);
    }

    if (activeAction2->animationClip->animation->numBones != bonesToInterpolate2.size()) {
        bonesToInterpolate2.clear();
        bonesToInterpolate2.resize(activeAction2->animationClip->animation->numBones);
    }

    float interpolation = activeTransition->transitionProcess / activeTransition->transitionTime;
    interpolation *= 2.f;
    interpolation -= 1.f;
    interpolation = glm::sin(interpolation * (glm::pi<float>() / 2.f));
    interpolation += 1.f;
    interpolation /= 2.f;

    glm::vec3 finalPos = position1 + position1;
    glm::mat4 matrixPos = glm::mat4(1.f);
    matrixPos[3].x = finalPos.x;
    matrixPos[3].y = finalPos.y;
    matrixPos[3].z = finalPos.z;

    skeleton->skeletonBones[0]->globalTx = skeleton->skeletonBones[0]->localTx * matrixPos;
    bones[0] = skeleton->skeletonBones[0]->globalTx * skeleton->skeletonBones[0]->inversed;

    for (uint32_t i = 1; i < size; ++i) {
        float interpolation = activeTransition->transitionProcess / activeTransition->transitionTime;
        interpolation *= 2.f;
        interpolation -= 1.f;
        interpolation = glm::sin(interpolation * (glm::pi<float>() / 2.f));
        interpolation += 1.f;
        interpolation /= 2.f;

        // Clamp interpolation.
        if (interpolation > 0.999f)
            interpolation = 0.999f;
        else if (interpolation < 0.001f)
            interpolation = 0.001f;

        // Convert to quaternion to interpolate animation then back to matrix.
        glm::quat rotation1 = glm::quat_cast(bonesToInterpolate1[i]);
        glm::quat rotation2 = glm::quat_cast(bonesToInterpolate2[i]);
        glm::quat finalRot = glm::slerp(rotation1, rotation2, interpolation);

        glm::mat4 matrixRot = glm::mat4(finalRot);

        skeleton->skeletonBones[i]->globalTx = skeleton->skeletonBones[skeleton->skeletonBones[i]->parentId]->globalTx * skeleton->skeletonBones[i]->localTx * matrixRot;
        bones[i] = skeleton->skeletonBones[i]->globalTx * skeleton->skeletonBones[i]->inversed;
    }
}

void AnimationController::SetBool(const std::string& name, bool value) {
    for (std::size_t i = 0; i < controller->boolMap.size(); ++i)
        if (strcmp(name.c_str(), controller->boolMap[i]->name) == 0) {
            controller->boolMap[i]->value = value;
            return;
        }
}

void AnimationController::SetFloat(const std::string& name, float value) {
    for (std::size_t i = 0; i < controller->floatMap.size(); ++i)
        if (strcmp(name.c_str(), controller->floatMap[i]->name) == 0) {
            controller->floatMap[i]->value = value;
            return;
        }
}

bool AnimationController::GetBool(const std::string& name) {
    for (std::size_t i = 0; i < controller->boolMap.size(); ++i)
        if (strcmp(name.c_str(), controller->boolMap[i]->name) == 0)
            return controller->boolMap[i]->value;
}

float AnimationController::GetFloat(const std::string& name) {
    for (std::size_t i = 0; i < controller->floatMap.size(); ++i)
        if (strcmp(name.c_str(), controller->floatMap[i]->name) == 0)
            return controller->floatMap[i]->value;
}
