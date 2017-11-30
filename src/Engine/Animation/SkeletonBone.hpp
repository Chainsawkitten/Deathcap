#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include "../linking.hpp"

namespace Animation {
    /// Bone in bindpose.
    struct SkeletonBone {
        /// Save skeleton bone.
        /**
         * @param file File to save to.
         */
        ENGINE_API void Save(std::ofstream* file);

        /// Load skeleton bone.
        /**
         * @param file File to load from.
         */
        ENGINE_API void Load(std::ifstream* file);

        /// Local transformation of skeleton bone.
        glm::mat4 localTx;

        /// Global transformation of skeleton bone.
        glm::mat4 globalTx;

        /// Bone inversed.
        glm::mat4 inversed;

        /// Id of bone parent.
        uint32_t parentId;
    };
}
