#pragma once

#include <fstream>
#include <cstdint>
#include <assert.h>
#include <vector>
#include <Video/Geometry/VertexType/StaticVertex.hpp>
#include <Video/Geometry/VertexType/SkinVertex.hpp>

namespace Geometry {
    /// Handler for .asset format.
    /**
     * Start by using the Open() function.
     * The Open() function requries a filepath and
     * a mode READ/WRITE.
     * End by using the Close() function.
     */
    class AssetFileAnimation {
        public:
            /// Mode to use class. READ/WRITE.
            enum Mode {
                READ = 0,
                WRITE = 1
            };
            
            /// Dock
            struct Bone {
                uint32_t parent;
                uint32_t numRotationKeys;
                uint32_t * rotationKeys = nullptr;
                glm::mat4 * rotation = nullptr;

                ~Bone() {
                    if (rotationKeys != nullptr) {
                        delete[] rotationKeys;
                    }

                    if (rotation != nullptr) {
                        delete[] rotation;
                    }
                }

                void Save(std::ofstream * file) {
                    file->write(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(rotationKeys), sizeof(uint32_t) * numRotationKeys);
                    file->write(reinterpret_cast<char*>(rotation), sizeof(uint32_t) * numRotationKeys);
                }

                void Load(std::ifstream * file) {
                    file->read(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));

                    if (rotationKeys != nullptr) {
                        delete[] rotationKeys;
                    }
                    rotationKeys = new uint32_t[numRotationKeys];
                    file->read(reinterpret_cast<char*>(rotationKeys), sizeof(uint32_t) * numRotationKeys);

                    if (rotation != nullptr) {
                        delete[] rotation;
                    }
                    rotation = new glm::mat4[numRotationKeys];
                    file->read(reinterpret_cast<char*>(rotation), sizeof(uint32_t) * numRotationKeys);
                }
            };

            /// Dock
            struct Animation {
                char name[32];

                // Frames per second, ex 30.0f.
                float playbackSpeed;
                uint32_t numKeyframes;
                uint32_t numBones;
                Bone * bones = nullptr;

                ~Animation() {
                    if (bones != nullptr) {
                        delete[] bones;
                    }
                }

                void Save(std::ofstream * file) {
                    file->write(reinterpret_cast<char*>(&name), sizeof(char) * 32);
                    file->write(reinterpret_cast<char*>(&numKeyframes), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    for (int i = 0; i < numBones; ++i) {
                        bones[i].Save(file);
                    }
                }

                void Load(std::ifstream * file) {
                    file->read(reinterpret_cast<char*>(&name), sizeof(char) * 32);
                    file->read(reinterpret_cast<char*>(&numKeyframes), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    if (bones != nullptr) {
                        delete[] bones;
                    }
                    bones = new Bone[numBones];
                    for (int i = 0; i < numBones; ++i) {
                        bones[i].Load(file);
                    }
                }
            };

            /// DOCK
            struct BindPose {
                uint32_t numBones;
                uint32_t * parents = nullptr;
                glm::mat4 * localTx = nullptr;
                glm::mat4 * globalTx = nullptr;
                glm::mat4 * invBindPose = nullptr;


                ~BindPose() {
                    if (parents != nullptr) {
                        delete[] parents;
                    }

                    if (localTx != nullptr) {
                        delete[] localTx;
                    }

                    if (globalTx != nullptr) {
                        delete[] globalTx;
                    }

                    if (invBindPose != nullptr) {
                        delete[] invBindPose;
                    }
                }

                /// Save the bindpose to file.
                /**
                 * @param file File to save.
                 */
                void Save(std::ofstream * file) {
                    file->write(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&parents), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(localTx), sizeof(glm::mat4) * numBones);
                    file->write(reinterpret_cast<char*>(globalTx), sizeof(glm::mat4) * numBones);
                    file->write(reinterpret_cast<char*>(invBindPose), sizeof(glm::mat4) * numBones);

                }

                /// Load bind pose.
                /**
                 * @param file File to load.
                 */
                void Load(std::ifstream * file) {
                    file->read(reinterpret_cast<char*>(&numBones), sizeof(uint32_t));

                    if (parents != nullptr) {
                        delete[] parents;
                    }
                    parents = new uint32_t[numBones];
                    file->read(reinterpret_cast<char*>(parents), sizeof(uint32_t));

                    if (localTx != nullptr) {
                        delete[] localTx;
                    }
                    localTx = new glm::mat4[numBones];
                    file->read(reinterpret_cast<char*>(localTx), sizeof(glm::mat4) * numBones);

                    if (globalTx != nullptr) {
                        delete[] globalTx;
                    }
                    globalTx = new glm::mat4[numBones];
                    file->read(reinterpret_cast<char*>(globalTx), sizeof(glm::mat4) * numBones);

                    if (invBindPose != nullptr) {
                        delete[] invBindPose;
                    }
                    invBindPose = new glm::mat4[numBones];
                    file->read(reinterpret_cast<char*>(invBindPose), sizeof(glm::mat4) * numBones);
                }
            };

            /// Constructor.
            AssetFileAnimation();

            /// Destructor.
            ~AssetFileAnimation();

            /// Open a .asset file.
            /**
            * @param filepath Path of the file.
            * @param mode Use READ or WRITE.
            */
            bool Open(const char* filepath, Mode mode = READ);

            /// Close the opened file and clear data.
            void Close();

            /// Clear current data.
            void Clear();

        private:
            std::ifstream rFile;
            std::ofstream wFile;
    };
}