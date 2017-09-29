#include "AssetConverterSkeleton.hpp"
#include <glm/gtc/quaternion.hpp>
#include <Engine/Geometry/MathFunctions.hpp>
#include <Utility/Log.hpp>

AssetConverterSkeleton::AssetConverterSkeleton() {
}

AssetConverterSkeleton::~AssetConverterSkeleton() {
}

bool AssetConverterSkeleton::Convert(const char * filepath, const char * destination,
    bool bindpose) {

    success = true;
    errorString = "";

    const aiScene* aScene = aImporter.ReadFile(filepath, 0);

    if (!aScene->HasAnimations()) {
        errorString.append("Error: No animation in scene.");
        success = false;
        return false;
    }

    bones.clear();
    bones.shrink_to_fit();
    BoneRecursive(aScene->mRootNode, 0, aScene->mAnimations[0]);

    for (int i = 0; i < bones.size(); ++i) {
        Log() << i << "\t\t" << bones[i]->parent << "\n";
    }

    aImporter.FreeScene();

    return false;
}

bool AssetConverterSkeleton::Success() const {
    return false;
}

std::string & AssetConverterSkeleton::GetErrorString() {
    return errorString;
}

void AssetConverterSkeleton::BoneRecursive(aiNode * node, uint32_t parent, aiAnimation * anim) {
    bool foundAnimNode = false;

    for (unsigned int i = 0; i < anim->mNumChannels && !foundAnimNode; ++i) {
        if (anim->mChannels[i]->mNodeName == node->mName) {
            Geometry::AssetFileAnimation::Bone bone;

            bone.numRotationKeys = (uint32_t)anim->mChannels[i]->mNumRotationKeys;

            bone.rotationKeys = new uint32_t[anim->mChannels[i]->mNumRotationKeys];
            bone.rotation = new glm::mat4[anim->mChannels[i]->mNumRotationKeys];

            for (unsigned int j = 0; j < bone.numRotationKeys; ++j) {
                // Copy the keyframe from the timeline.
                bone.rotationKeys[j] = (uint32_t)anim->mChannels[i]->mRotationKeys[j].mTime;

                // Copy the rotation.
                glm::quat rot;
                Geometry::CpyQuat(rot, anim->mChannels[i]->mRotationKeys[j].mValue);
                bone.rotation[j] = glm::mat4(rot);
            }

            bone.parent = parent;
          // FIX THIS
            bones.push_back(&bone);
            foundAnimNode = true;
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        BoneRecursive(node->mChildren[i], bones.size() - 1, anim);
    }
}
