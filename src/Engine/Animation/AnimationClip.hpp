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
                glm::mat4* rotations = nullptr;
                uint32_t currentKeyIndex = 0;

                ~Bone() {
                    if (rotationKeys != nullptr)
                        delete[] rotationKeys;

                    if (rotations != nullptr)
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
                        file->write(reinterpret_cast<char*>(&rotations[i]), sizeof(float) * 16);
                }

                /// Load bone data.
                /**
                 * @param file File to load from.
                 */
                void Load(std::ifstream* file) {
                    file->read(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));

                    if (rotationKeys != nullptr)
                        delete[] rotationKeys;

                    if (rotations != nullptr)
                        delete[] rotations;

                    rotationKeys = new int32_t[numRotationKeys];
                    for (unsigned int i = 0; i < numRotationKeys; ++i) 
                        file->read(reinterpret_cast<char*>(&rotationKeys[i]), sizeof(int32_t));

                    rotations = new glm::mat4[numRotationKeys];
                    for (unsigned int i = 0; i < numRotationKeys; ++i)
                        file->read(reinterpret_cast<char*>(&rotations[i]), sizeof(float) * 16);
                }
            };

            /// Animation data.
            struct Animation {
                uint32_t numBones = 0;
                Bone* bones = nullptr;
                int32_t length = 0;
                float currentFrame = 0.0f;
                
                /// Save animation data.
                /**
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) {
                    file->write(reinterpret_cast<char*>(&length), sizeof(int32_t));
                    file->write(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    
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

                    if (bones != nullptr)
                        delete[] bones;

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
