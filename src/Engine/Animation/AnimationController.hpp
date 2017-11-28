#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "../Util/Node.hpp"
#include <map>
#include "../linking.hpp"
#include "../Manager/Managers.hpp"
#include "../Manager/ResourceManager.hpp"
#include <Utility/Log.hpp>

namespace Animation {
    class AnimationClip;
    class Skeleton;
}

namespace Animation {
    /// An animation loaded from a file.
    class AnimationController {
        public:
            /// Animaiton action node.
            struct AnimationAction : public Node {
                char animationClipName[512];
                bool isPlaybackModifierStatic = true;
                float playbackModifier = 1.0f;
                int32_t playbackModifierFloatIndex = -1;
                bool repeat = true;
                Animation::AnimationClip* animationClip = nullptr;

                /// Destructor.
                virtual ~AnimationAction() {
                    if (animationClip != nullptr)
                        Managers().resourceManager->FreeAnimationClip(animationClip);
                }

                /// Save the animation action node.
                /**
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) override {
                    Node::Save(file);
                    file->write(reinterpret_cast<char*>(animationClipName), 512);
                    file->write(reinterpret_cast<char*>(&playbackModifier), sizeof(uint32_t));
                    file->write(reinterpret_cast<char*>(&repeat), sizeof(bool));
                }

                /// Load the animation action node.
                /**
                 * @param file File to load from.
                 */
                void Load(std::ifstream* file) override {
                    Node::Load(file);
                    file->read(reinterpret_cast<char*>(animationClipName), 512);
                    file->read(reinterpret_cast<char*>(&playbackModifier), sizeof(uint32_t));
                    file->read(reinterpret_cast<char*>(&repeat), sizeof(bool));

                    if (animationClip != nullptr)
                        Managers().resourceManager->FreeAnimationClip(animationClip);

                    animationClip = Managers().resourceManager->CreateAnimationClip(animationClipName);
                }
            };

            /// Animation transition node.
            struct AnimationTransition : public Node {
                bool isStatic = true;
                int32_t transitionBoolIndex = -1;
                float transitionTime = 0.5f;
                float transitionProcess = 0.0f;

                /// Save the animation transition node.
                /**
                 * @param file File to save to.
                 */
                void Save(std::ofstream* file) override {
                    Node::Save(file);
                    file->write(reinterpret_cast<char*>(&isStatic), sizeof(bool));
                    file->write(reinterpret_cast<char*>(&transitionBoolIndex), sizeof(int32_t));
                    file->write(reinterpret_cast<char*>(&transitionTime), sizeof(float));
                }

                /// Load the animation transition node.
                /**
                 * @param file File to load from.
                 */
                void Load(std::ifstream* file) override {
                    Node::Load(file);
                    file->read(reinterpret_cast<char*>(&isStatic), sizeof(bool));
                    file->read(reinterpret_cast<char*>(&transitionBoolIndex), sizeof(int32_t));
                    file->read(reinterpret_cast<char*>(&transitionTime), sizeof(float));
                }
            };

            /// Used to map bools.
            struct BoolItem {
                char name[128] = "NewBool\0";
                bool value = true;
            };

            /// Used to map floats.
            struct FloatItem {
                char name[128] = "NewFloat\0";
                float value = 1.f;
            };

            /// Default constructor. 
            AnimationController() = default;
            
            /// Destructor.
            ~AnimationController();

            /// Save animation controller.
            /**
             * @param path Save location.
             */
            ENGINE_API void Save(const std::string& path);

            /// Load animation controller.
            /**
             * @param name Load from name.
             */
            ENGINE_API void Load(const std::string& name);

            /// Vector with the animation nodes.
            std::vector<Node*> animationNodes;

            /// Used to map bools for animation system.
            std::vector<BoolItem*> boolMap;

            /// Used to map floats for animation system.
            std::vector<FloatItem*> floatMap;

            /// Skeleton.
            Skeleton* skeleton;

            /// Name of resource.
            std::string name;

            /// The folder containing the model file.
            std::string path;

        private:
            enum NodeType {
                ACTION = 0,
                TRANSITION = 1
            };

            void Clear();
    };
}
