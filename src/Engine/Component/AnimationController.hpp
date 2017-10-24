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
            void UpdateAnimation(float deltaTime);

            /// Vector with the final calculated bones.
            std::vector<glm::mat4> bones;

            /// The controller.
            Animation::AnimationController* controller = nullptr;

            /// The skeleton.
            Animation::Skeleton* skeleton = nullptr;

        private:
            Animation::AnimationController::AnimationAction* activeAction1 = nullptr;
            Animation::AnimationController::AnimationAction* activeAction2 = nullptr;
            Animation::AnimationController::AnimationTransition* activeTransition = nullptr;
    };
}