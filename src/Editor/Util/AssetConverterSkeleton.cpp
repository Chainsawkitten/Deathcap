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

    bones.clear();
    bones.shrink_to_fit();

    if (!aScene->HasAnimations()) {
        errorString.append("Error: No animation in scene.");
        success = false;
        return false;
    }

    // Push back the bones.
    for (int i = 0; i < aScene->mMeshes[0]->mNumBones; ++i) {
        bones.push_back(aScene->mMeshes[0]->mBones[i]->mName.C_Str());
    }

    currentId = -1;
    BoneRecursive(aScene->mRootNode, -1);

    for (int i = 0; i < bones.size(); ++i) {
        Log() << "IDS: " << ids[i] << "\tPARENTS: " << parents[i] << "\n";
    }

    for (int i = 1; i < bones.size(); ++i) {
        Log() << "CHILD: " << children[i] << "\tCHILD ID: " << ids[i] << "\tPARENT ID: " << parents[i] << "\n";
    }


    return false;
}

bool AssetConverterSkeleton::Success() const {
    return false;
}

std::string & AssetConverterSkeleton::GetErrorString() {
    return errorString;
}

void AssetConverterSkeleton::BoneRecursive(aiNode * node, int parent) {
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        aiNode * child = node->mChildren[i];

        for (unsigned int j = 0; j < bones.size(); ++j) {
            if (child->mName.C_Str() == bones[j]) {
                children.push_back(child->mName.C_Str());
                parents.push_back(parent);
                ++currentId;
                ids.push_back(currentId);
                break;
            }
        }
        BoneRecursive(child, currentId);
    }
}
