#pragma once

#include <glm/glm.hpp>
#include <fstream>
#include "../linking.hpp"

namespace Animation {
    struct Bone;
}

namespace Animation {
    /// Animation data.
    struct Animation {
        /// Destructor.
        ENGINE_API ~Animation();

        /// Save animation data.
        ENGINE_API void Save(std::ofstream* file);

        /// Load animation data.
        ENGINE_API void Load(std::ifstream* file);

        /// Number of bones.
        uint32_t numBones = 0;

        /// Bones array.
        Bone* bones = nullptr;

        /// Lenght of animation.
        int32_t length = 0;

        /// Current keyframe.
        float currentFrame = 0.f;

        /// Number of root positions.
        uint32_t numRootPositions = 0;

        /// Root position key array.
        int32_t* rootPositionKeys = nullptr;

        /// Root position vectors.
        glm::vec3* rootPositions = nullptr;

        /// Current root key index.
        int32_t currentRootKeyIndex = 0;
    };
}
