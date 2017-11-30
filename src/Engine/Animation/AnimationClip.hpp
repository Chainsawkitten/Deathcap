#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Utility/Log.hpp>
#include "../linking.hpp"

namespace Animation {
    /// An animation loaded from a file.
    class AnimationClip {
        public:
            /// Bone data.
            struct Bone {
                uint32_t parent = 0;
                uint32_t numRotationKeys = 0;
                int32_t* rotationKeys = nullptr;
                glm::quat* rotations = nullptr;
                uint32_t currentKeyIndex = 0;

                /// Destructor.
                ~Bone() {
                    if (rotationKeys)
                        delete[] rotationKeys;

                    if (rotations)
                        delete[] rotations;
                }

                /// Save bone data.
                /**  
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) {
                    file->write(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));

                    for (unsigned int i = 0; i < numRotationKeys; ++i)  
                        file->write(reinterpret_cast<char*>(&rotationKeys[i]), sizeof(int32_t));
                    
                    for (unsigned int i = 0; i < numRotationKeys; ++i) 
                        file->write(reinterpret_cast<char*>(&rotations[i]), sizeof(glm::quat));
                }

                /// Load bone data.
                /**
                 * @param file File to load from.
                 */
                void Load(std::ifstream* file) {
                    file->read(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));

                    if (rotationKeys)
                        delete[] rotationKeys;

                    if (rotations)
                        delete[] rotations;

                    rotationKeys = new int32_t[numRotationKeys];
                    for (unsigned int i = 0; i < numRotationKeys; ++i) 
                        file->read(reinterpret_cast<char*>(&rotationKeys[i]), sizeof(int32_t));

                    rotations = new glm::quat[numRotationKeys];
                    for (unsigned int i = 0; i < numRotationKeys; ++i)
                        file->read(reinterpret_cast<char*>(&rotations[i]), sizeof(glm::quat));
                }
            };

            /// Animation data.
            struct Animation {
                uint32_t numBones = 0;
                Bone* bones = nullptr;
                int32_t length = 0;
                float currentFrame = 0.0f;
                uint32_t numRootPositions = 0;
                int32_t* rootPositionKeys = nullptr;
                glm::vec3* rootPositions = nullptr;
                int32_t currentRootKeyIndex = 0;
                
                /// Destructor.
                ~Animation() {
                    if (bones)
                        delete[] bones;

                    if (rootPositionKeys)
                        delete[] rootPositionKeys;

                    if (rootPositions)
                        delete[] rootPositions;
                }

                /// Save animation data.
                /**
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) {
                    file->write(reinterpret_cast<char*>(&length), sizeof(int32_t));
                    file->write(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&numRootPositions), sizeof(int32_t));
                    
                    file->write(reinterpret_cast<char*>(rootPositionKeys), sizeof(int32_t) * numRootPositions);
                    file->write(reinterpret_cast<char*>(rootPositions), sizeof(glm::vec3) * numRootPositions);

                    for (unsigned int i = 0; i < numBones; ++i)
                        bones[i].Save(file);
                }

                /// Load animation data.
                /**
                 * @param file File to load from.
                 */
                void Load(std::ifstream* file) {
                    file->read(reinterpret_cast<char*>(&length), sizeof(int32_t));
                    file->read(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numRootPositions), sizeof(int32_t));

                    if (bones)
                        delete[] bones;

                    if (rootPositionKeys)
                        delete[] rootPositionKeys;

                    if (rootPositions)
                        delete[] rootPositions;

                    rootPositionKeys = new int32_t[numRootPositions];
                    file->read(reinterpret_cast<char*>(rootPositionKeys), sizeof(int32_t) * numRootPositions);

                    rootPositions = new glm::vec3[numRootPositions];
                    file->read(reinterpret_cast<char*>(rootPositions), sizeof(glm::vec3) * numRootPositions);

                    bones = new Bone[numBones];                    
                    for (unsigned int i = 0; i < numBones; ++i)
                        bones[i].Load(file);
                }
            };

            /// Default constructor.
            AnimationClip() = default;

            /// Load.
            /**
             * @param name Name of animation.
             */
            ENGINE_API void Load(const std::string& name);       

            Animation* animation = nullptr;

            /// Path of animation clip.
            std::string path;

            /// Name of animation clip.
            std::string name;
    };
}
