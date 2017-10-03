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
                }

                void Save(std::ofstream * file) {
                }

                void Load(std::ifstream * file) {
                }
            };


            void Load(const std::string& name);

            std::string name;
        private:

    };
}
