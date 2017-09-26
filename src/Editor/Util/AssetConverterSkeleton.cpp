#include "AssetConverterSkeleton.hpp"
#include <glm/gtc/quaternion.hpp>
#include <Engine/Geometry/MathFunctions.hpp>
#include <Utility/Log.hpp>

AssetConverterSkeleton::AssetConverterSkeleton() {
}

AssetConverterSkeleton::~AssetConverterSkeleton() {
}

bool AssetConverterSkeleton::Convert(const char * filepath, const char * destination,
    bool triangulate, bool importNormals, bool importTangents) {

    success = true;
    errorString = "";

    const aiScene* aScene = aImporter.ReadFile(filepath, 0);

    if (!aScene->HasAnimations()) {
        errorString.append("Error: No animation in scene.");
        success = false;
        return false;
    }

    Recursive(aScene->mRootNode);

    Geometry::AssetFileAnimation::Bone * bones = new Geometry::AssetFileAnimation::Bone[nodes.size()];

    // Find parent.
    // @todo Optimize? If time. It's offline so not very important.
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Log() << nodes[i]->mName.C_Str() << "\n";
        for (unsigned int j = 0; j < nodes.size(); ++j) {
            if (nodes[i] == nodes[j]->mParent) {
                bones[i].parent = j;
                break;
            }
        }
    }

    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < aScene->mAnimations[0]->mNumChannels; ++j) {
            if (strcmp(nodes[i]->mName.C_Str(), aScene->mAnimations[0]->mChannels[j]->mNodeName.C_Str()) == 0) {
                aiNodeAnim * node = aScene->mAnimations[0]->mChannels[j];
                bones[i].numRotationKeys = node->mNumRotationKeys;
                bones[i].rotationKeys = new uint32_t[bones[i].numRotationKeys];
                bones[i].rotation = new glm::mat4[bones[i].numRotationKeys];

                for (unsigned int r = 0; r < bones[i].numRotationKeys; ++r) {
                    bones[i].rotationKeys[r] = (uint32_t)node->mRotationKeys->mTime;
                    glm::quat rot;
                    Geometry::CpyQuat(rot, node->mRotationKeys->mValue);
                    bones[i].rotation[r] = glm::mat4(rot);
                }

                Log() << nodes[i]->mName.C_Str() << "   " << aScene->mAnimations[0]->mChannels[j]->mNodeName.C_Str() << "   " << bones[i].numRotationKeys << "\n";
            }
        }
    }

    Geometry::AssetFileAnimation::Animation * anim = new Geometry::AssetFileAnimation::Animation;

    anim->bones = bones;
    anim->playbackSpeed = (uint32_t)aScene->mAnimations[0]->mTicksPerSecond;
    anim->numBones = nodes.size();


    //aScene->mAnimations
    aScene->mRootNode;

    return false;
}

bool AssetConverterSkeleton::Success() const {
    return false;
}

std::string & AssetConverterSkeleton::GetErrorString() {
    return errorString;
}

void AssetConverterSkeleton::Recursive(aiNode * node) {
    nodes.push_back(node);

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        Recursive(node->mChildren[i]);
    }
}
