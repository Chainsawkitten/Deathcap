#pragma once

#include "SuperComponent.hpp"
#include "../Animation/AnimationController.hpp"
#include <string>
#include <glm/glm.hpp>

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

            Animation::AnimationController* controller = nullptr;

            std::vector<glm::mat4>& GetBones();

        private:

    };
}