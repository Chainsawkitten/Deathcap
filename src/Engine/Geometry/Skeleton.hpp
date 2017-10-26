#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "../linking.hpp"

struct aiScene;
struct aiNode;

namespace Geometry {
    class Animation;
}

/// A skeleton loaded from a file.
class Skeleton {
        public:
    /// Create new empty skeleton.
    /**
         * The created skeleton has to be loaded later using Load.
         */
    ENGINE_API Skeleton();

    /// Create new skeleton.
    /**
         * @param aScene Pointer to assimp scene.
         */
    ENGINE_API Skeleton(const aiScene* aScene);

    /// Destructor.
    ENGINE_API ~Skeleton();

    /// Load skeleton.
    /**
         * @param aScene Pointer to assimp scene.
         */
    ENGINE_API void Load(const aiScene* aScene);

    /// Get bone index.
    /**
         * @param name Bone name.
         * @return Bone index, if not found -1.
         */
    ENGINE_API std::size_t FindBoneIndex(const std::string& name);

    /// Get number of bones.
    /**
         * @return Number of bones.
         */
    ENGINE_API std::size_t GetNumBones() const;

    /// Animate skeleton.
    /**
         * Use GetFinalTransformations after animation to get matrices.
         * @param animation Animation to animate skeleton.
         * @param timeInSeconds Time to find animation frame.
         */
    ENGINE_API void Animate(const Geometry::Animation* animation, const float timeInSeconds);

    /// Update skeleton to bind pose.
    /**
         * Use GetFinalTransformations after animation to get matrices.
         */
    ENGINE_API void BindPose();

    /// Get vector of transformations of skeleton.
    /**
         * Final transformations are updated when skeleton is animated.
         * @return Vector of bone transformations.
         */
    ENGINE_API const std::vector<glm::mat4>& GetFinalTransformations() const;

    /// Get vector of inverse transpose transformations of skeleton.
    /**
         * Final transformations are updated when skeleton is animated.
         * @return Vector of bone inverse transpose transformations.
         */
    ENGINE_API const std::vector<glm::mat3>& GetFinalTransformationsIT() const;

        private:
    struct Node {
        std::string name;
        glm::mat4 transformation;
        Node* parent;
        std::vector<Node> children;
    };

    static void LoadNodeTree(aiNode* aNode, Node* node, Node* parentNode);
    void ReadNodeHeirarchy(const Geometry::Animation* animation, float animationTime, Node* node, const glm::mat4& parentTransform);
    const glm::mat4* FindBone(const std::string& name) const;

    glm::mat4 globalInverseTransform;
    Node rootNode;
    std::vector<glm::mat4> bones;
    std::vector<glm::mat4> finalTransforms;
    std::vector<glm::mat3> finalTransformsIT;
    std::map<std::string, std::size_t> boneIndexMap;
};
