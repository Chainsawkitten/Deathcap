#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

namespace Animation {
    /// An animation loaded from a file.
    class AnimationClip {
        public:
            /// Dock
            struct Bone {
                Bone * parent;
                uint32_t numChildren;
                Bone * children;
                uint32_t numRotationKeys;
                uint32_t * rotationKeys = nullptr;
                glm::mat4 * rotations = nullptr;

                ~Bone() {
                    if (rotationKeys != nullptr) {
                        delete[] rotationKeys;
                    }

                    if (rotations != nullptr) {
                        delete[] rotations;
                    }
                }

                void Save(std::ofstream * file) {
                    file->write(reinterpret_cast<char*>(&numChildren), sizeof(uint32_t));
                    for (unsigned int i = 0; i < numChildren; ++i) {
                        children->Save(file);
                    }
                    file->write(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(rotationKeys), sizeof(uint32_t) * numRotationKeys);
                    file->write(reinterpret_cast<char*>(rotations), sizeof(uint32_t) * numRotationKeys);
                }

                void Load(std::ifstream * file) {
                    file->read(reinterpret_cast<char*>(&parent), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&numRotationKeys), sizeof(uint32_t));

                    if (rotationKeys != nullptr) {
                        delete[] rotationKeys;
                    }
                    rotationKeys = new uint32_t[numRotationKeys];
                    file->read(reinterpret_cast<char*>(rotationKeys), sizeof(uint32_t) * numRotationKeys);

                    if (rotations != nullptr) {
                        delete[] rotations;
                    }
                    rotations = new glm::mat4[numRotationKeys];
                    file->read(reinterpret_cast<char*>(rotations), sizeof(uint32_t) * numRotationKeys);
                }
            };


            void Load(const std::string& name);

            std::string name;
        private:

    };
}
