#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "../linking.hpp"

namespace Animation {
    /// An animation loaded from a file.
    /// @todo Implement this class.
    class Skeleton {
        public:
            struct SkeletonBone {
                glm::mat4 localTx;
                glm::mat4 globalTx = glm::mat4(1.0f);
                glm::mat4 inversed = glm::mat4(1.0f);
                uint32_t parentId = 0;
            };

            /// Constructor.
            ENGINE_API Skeleton();

            /// Save skeleton.
            /**
             * @param name Name of file.
             */
            ENGINE_API void Save(std::string name);

            /// Load skeleton.
            /** 
             * @param name Name of skeleton to load.
             */ 
            ENGINE_API void Load(std::string name);

            /// Bones of skeleton.
            std::vector<SkeletonBone*> skeletonBones;

            /// Path to skeleton.
            std::string path;

            /// Name of the resource.
            std::string name;

        private:
            void InitSkeleton();
    };
}
