#pragma once

#include "SuperComponent.hpp"

#include <string>

namespace Component {
    /// Animation controller.
    class AnimationController : public SuperComponent {
        public:
            /// Create new animation controller component.
            /**
            * @param entity Pointer to which entity this component corresponds.
            */
            AnimationController(Entity * entity);

            /// Save the component.
            /**
            * @return JSON value to be stored on disk.
            */
            Json::Value Save() const override;

            /// Load component from JSON node.
            /**
            * @param node JSON node to load from.
            */
            void Load(const Json::Value& node) override;

            void PlayAnimation(std::string animation);

        private:

    };
}