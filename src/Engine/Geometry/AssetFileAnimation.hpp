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
            AssetFileAnimation();
            ~AssetFileAnimation();
            
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
                    file->read(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                }
            };

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

                    
                }
            };

            struct BindPose {


                void Save(std::ofstream * file) {

                }

                void Load(std::ifstream * file) {

                }
            };



        private:
            std::ifstream rFile;
            std::ofstream wFile;
    };
}