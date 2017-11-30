#pragma once

#include "SuperComponent.hpp"
#include <string>
#include <glm/glm.hpp>
#include "../Animation/AnimationController.hpp"

namespace Component {
    /// Animation controller.
    class AnimationController : public SuperComponent {
        public:
            /// Create new animation controller component.
            AnimationController();

            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;

            /// Update the animation controller.
            /**
             * @param deltaTime Time between frames.
             */
            ENGINE_API void UpdateAnimation(float deltaTime);

            /// Set a bool in the state machine.
            /**
             * @param name Name of the bool.
             * @param value Value to set.
             */
            ENGINE_API void SetBool(const std::string& name, bool value);

            /// Set a float in the state machine.
            /**
             * @param name Name of the float.
             * @param value Value to set.
             */
            ENGINE_API void SetFloat(const std::string& name, float value);

            /// Get a bool in the state machine.
            /**
             * @param name Name of the bool.
             * @return Value of the bool.
             */
            ENGINE_API bool GetBool(const std::string& name);

            /// Get a float in the state machine.
            /**
             * @param name Name of the float.
             * @return Value of the float.
             */
            ENGINE_API float GetFloat(const std::string& name);

            /// Vector with the final calculated bones.
            std::vector<glm::mat4> bones;

            /// The controller.
            Animation::AnimationController* controller = nullptr;

            /// The skeleton.
            Animation::Skeleton* skeleton = nullptr;

        private:
            void Animate(float deltaTime, Animation::AnimationController::AnimationAction* action, unsigned int skeletonId = 0);
            void Blend(float deltaTime);

            Animation::AnimationController::AnimationAction* activeAction1 = nullptr;
            Animation::AnimationController::AnimationAction* activeAction2 = nullptr;
            Animation::AnimationController::AnimationTransition* activeTransition = nullptr;

            std::vector<glm::mat4> bonesToInterpolate1;
            std::vector<glm::mat4> bonesToInterpolate2;

            bool isBlending = false;
    };
}
