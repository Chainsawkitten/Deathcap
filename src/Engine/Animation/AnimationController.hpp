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
            struct Node {
                uint32_t index;
                char name[128];
                uint32_t numInputSlots = 0;
                uint32_t inputIndex[8];
                uint32_t numOutputSlots = 0;
                uint32_t outputIndex[8];
                glm::vec2 pos = glm::vec2(0.0f);
                glm::vec2 size = glm::vec2(250, 100);

                virtual void Save(std::ofstream* file) {
                    file->write(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(name), 128);
                    file->write(reinterpret_cast<char*>(&numInputSlots), sizeof(uint32_t));
                }

                virtual void Load(std::ifstream* file) {

                }
            };

            struct AnimationAction : public Node {
                char animationClip[128];
                float playbackModifier = 1.0f;
                bool repeat = true;

                virtual void Save(std::ofstream* file) override {
                    Node::Save(file);
                    file->write(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                }

                virtual void Load(std::ifstream* file) override {
                    Node::Load(file);
                    file->read(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                }
            };

            struct AnimationTransition : public Node {
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


            std::vector<AnimationAction*> animationAction;

            std::vector<AnimationTransition*> animationTransitions;

            /// 
            std::string name;

            /// The folder containing the model file.
            std::string path;

        private:

    };
}
