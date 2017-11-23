#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "../linking.hpp"

namespace Animation {
    /// Skeleton, bindpose of animation.
    class Skeleton {
        public:
            struct SkeletonBone {
                glm::mat4 localTx;
                glm::mat4 globalTx;
                glm::mat4 inversed;
                uint32_t parentId;
                
                /// Save skeleton bone.
                /**
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) {
                    file->write(reinterpret_cast<char*>(&localTx), sizeof(float) * 16);
                    file->write(reinterpret_cast<char*>(&globalTx), sizeof(float) * 16);
                    file->write(reinterpret_cast<char*>(&inversed), sizeof(float) * 16);
                    file->write(reinterpret_cast<char*>(&parentId), sizeof(uint32_t));
                }

                /// Load skeleton bone.
                /**
                * @param file File to load from.
                */
                void Load(std::ifstream* file) {
                    file->read(reinterpret_cast<char*>(&localTx), sizeof(float) * 16);
                    file->read(reinterpret_cast<char*>(&globalTx), sizeof(float) * 16);
                    file->read(reinterpret_cast<char*>(&inversed), sizeof(float) * 16);
                    file->read(reinterpret_cast<char*>(&parentId), sizeof(uint32_t));
                }
            };

            /// Constructor.
            ENGINE_API Skeleton();

            /// Save skeleton.
            /**
             * @param name Name of file.
             */
            ENGINE_API void Save(const std::string& name);

            /// Load skeleton.
            /** 
             * @param name Name of skeleton to load.
             */ 
            ENGINE_API void Load(const std::string& name);

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
