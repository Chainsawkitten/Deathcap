#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

namespace Animation {
    class AnimationClip;
    class Skeleton;
}

namespace Animation {
    /// An animation loaded from a file.
    class AnimationController {
        public:
            struct AnimationNode {
                uint32_t index;
                char nodeName[128];
                uint32_t numInputSlots;
                uint32_t inputIndex[8];
                uint32_t numOutputSlots;
                uint32_t outputIndex[8];

                virtual void Save(std::ofstream* file) = 0;
                virtual void Load(std::ifstream* file) = 0;
            };

            struct AnimationAction : public AnimationNode {
                uint32_t index;
                char nodeName[128];
                char animationClip[128];
                float playbackModifier = 1.0f;
                bool repeat = true;
                glm::vec2 pos;
                glm::vec2 size = glm::vec2(250, 100);

                virtual void Save(std::ofstream* file) override {
                    file->write(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                }

                virtual void Load(std::ifstream* file) override {
                    file->read(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                }
            };

            struct AnimationTransition : public AnimationNode {
                uint32_t in;
                float inTransitionValue;
                uint32_t out;
                float outTransitionValue;
                glm::vec2 pos;

                virtual void Save(std::ofstream* file) override {
                    file->write(reinterpret_cast<char*>(&in), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&inTransitionValue), sizeof(float));
                    file->write(reinterpret_cast<char*>(&out), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&outTransitionValue), sizeof(float));
                    file->write(reinterpret_cast<char*>(&pos), sizeof(glm::vec2));
                }

                virtual void Load(std::ifstream* file) override {
                    file->read(reinterpret_cast<char*>(&in), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&inTransitionValue), sizeof(float));
                    file->read(reinterpret_cast<char*>(&out), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&outTransitionValue), sizeof(float));
                    file->read(reinterpret_cast<char*>(&pos), sizeof(glm::vec2));
                }
            };

            void Save(const std::string& name);
            void Load(const std::string& name);

            std::string name;

            std::vector<AnimationAction*> animationAction;
            std::vector<AnimationTransition*> animationTransitions;

        private:

    };
}
