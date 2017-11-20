#include "AssetConverterSkeleton.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Geometry/MathFunctions.hpp>
#include <Engine/Animation/Skeleton.hpp>
#include <Utility/Log.hpp>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

AssetConverterSkeleton::AssetConverterSkeleton() {
}

AssetConverterSkeleton::~AssetConverterSkeleton() {
}

bool AssetConverterSkeleton::Convert(const char* filepath, const char* destination, bool isSkeleton) {

    success = true;
    errorString = "";

    const aiScene* aScene = aImporter.ReadFile(filepath, 0);

    bones.clear();
    bones.shrink_to_fit();

    if (!aScene->HasAnimations()) {
        errorString.append("Error: No animation in scene.");
        success = false;

        // Free memory.
        aImporter.FreeScene();
        return false;
    }

    // Push back the bones.
    for (unsigned int i = 0; i < aScene->mMeshes[0]->mNumBones; ++i)
        if (aScene->mMeshes[0]->mBones[i]->mName.C_Str() != aScene->mMeshes[0]->mName.C_Str())
            bones.push_back(aScene->mMeshes[0]->mBones[i]->mName.C_Str());

    currentId = -1;
    SceneRecursive(aScene->mRootNode, -1);

    if (isSkeleton) {
        Animation::Skeleton skeleton;
        for (unsigned int i = 0; i < aScene->mAnimations[0]->mNumChannels; ++i) {
            auto boneIndex = 0;
            for (auto j = 0; j < aScene->mAnimations[0]->mNumChannels; ++j)
                if (aScene->mAnimations[0]->mChannels[j]->mNodeName.C_Str() == bones[i]) {
                    boneIndex = j;
                    break;
                }

            aiNodeAnim* channel = aScene->mAnimations[0]->mChannels[boneIndex];
            Animation::Skeleton::SkeletonBone* bone = new Animation::Skeleton::SkeletonBone;
            bone->parentId = (uint32_t)parents[i];

            glm::mat4 modelMatrix(1.0f);

            glm::quat rot;
            rot.x = channel->mRotationKeys[0].mValue.x;
            rot.y = channel->mRotationKeys[0].mValue.y;
            rot.z = channel->mRotationKeys[0].mValue.z;
            rot.w = channel->mRotationKeys[0].mValue.w;

            glm::mat4 matRot(rot);

            glm::vec3 pos;
            pos.x = channel->mPositionKeys[0].mValue.x;
            pos.y = channel->mPositionKeys[0].mValue.y;
            pos.z = channel->mPositionKeys[0].mValue.z;
            modelMatrix = glm::translate(modelMatrix, pos);

            glm::mat4 scaleMatrix(1.f);
            glm::vec3 scale(1.f, 1.f, 1.f);
            scaleMatrix = glm::scale(scaleMatrix, scale);

            bone->localTx = modelMatrix * (matRot * scaleMatrix);

            skeleton.skeletonBones.push_back(bone);
        }

        // Save to file.
        skeleton.Save((std::string(destination)).c_str());
    } else {
        Animation::AnimationClip::Animation anim;
        anim.numBones = bones.size();
        anim.bones = new Animation::AnimationClip::Bone[aScene->mAnimations[0]->mNumChannels];
        anim.length = (uint32_t)aScene->mAnimations[0]->mChannels[aScene->mAnimations[0]->mChannels[1]->mNumRotationKeys - 1]->mRotationKeys->mTime;

        for (unsigned int i = 0; i < bones.size(); ++i) {
            auto boneIndex = 0;
            bool foundMatch = false;
            for (auto j = 0; j < aScene->mAnimations[0]->mNumChannels; ++j)
                if (aScene->mAnimations[0]->mChannels[j]->mNodeName.C_Str() == bones[i])
                    boneIndex = j;


            aiNodeAnim* channel = aScene->mAnimations[0]->mChannels[boneIndex];
            anim.bones[i].parent = (uint32_t)parents[i];
            anim.bones[i].numRotationKeys = channel->mNumRotationKeys;
            anim.bones[i].rotationKeys = new int32_t[channel->mNumRotationKeys];
            anim.bones[i].rotations = new glm::mat4[channel->mNumRotationKeys];

            for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j) {
                anim.bones[i].rotationKeys[j] = channel->mRotationKeys[j].mTime;
                glm::quat rot;
                rot.x = channel->mRotationKeys[j].mValue.x;
                rot.y = channel->mRotationKeys[j].mValue.y;
                rot.z = channel->mRotationKeys[j].mValue.z;
                rot.w = channel->mRotationKeys[j].mValue.w;

                glm::vec3 pos;
                pos.x = channel->mPositionKeys[j].mValue.x;
                pos.y = channel->mPositionKeys[j].mValue.y;
                pos.z = channel->mPositionKeys[j].mValue.z;

                glm::mat4 posMatrix(1.0f);
                posMatrix = glm::translate(posMatrix, pos);

                glm::mat4 scaleMatrix(1.0f);
                glm::vec3 scale(1.f, 1.f, 1.f);
                scaleMatrix = glm::scale(scaleMatrix, scale);
                anim.bones[i].rotations[j] = posMatrix * (glm::mat4(rot) * scaleMatrix);
            }
        }

        std::ofstream file((std::string(destination)).c_str(), std::ios::binary);
        anim.Save(&file);
        file.close();
    }

    // Free memory.
    aImporter.FreeScene();

    return true;
}

bool AssetConverterSkeleton::Success() const {
    return success;
}

const std::string& AssetConverterSkeleton::GetErrorString() {
    return errorString;
}

void AssetConverterSkeleton::SceneRecursive(aiNode* node, int parent) {
    if (!node)
        return;

    int thisId = currentId;

    for (auto i = 0; i < node->mNumChildren; ++i) {
        aiNode* child = node->mChildren[i];
        bool foundBone = false;
        for (auto j = 0; j < bones.size(); ++j)
            if (child->mName.C_Str() == bones[j]) {
                children.push_back(child->mName.C_Str());
                ids.push_back(0);
                ++currentId;
                parents.push_back(-1);
                BoneRecursive(child, 0);
            }

        if (!foundBone)
            SceneRecursive(child, thisId);
    }
}

void AssetConverterSkeleton::BoneRecursive(aiNode* node, int parent) {
    if (!node)
        return;

    int thisId = currentId;

    for (auto i = 0; i < node->mNumChildren; ++i) {
        aiNode* child = node->mChildren[i];
        bool foundBone = false;
        for (auto j = 0; j < bones.size() && !foundBone; ++j) {
            if (child->mName.C_Str() == bones[j]) {
                children.push_back(child->mName.C_Str());
                ids.push_back(thisId);

                for (auto k = 0; k < bones.size(); ++k)
                    if (bones[k] == child->mParent->mName.C_Str()) {
                        parents.push_back(k);
                        break;
                    }

                ++currentId;
                foundBone = true;
                BoneRecursive(child, thisId);
            }
        }

        if (!foundBone)
            return;
    }
}
