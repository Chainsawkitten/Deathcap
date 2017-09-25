#pragma once

#include <functional>

namespace Physics {

    class RigidBody;

    class ITrigger {
        public:
            virtual void OnEnter(RigidBody& body, std::function<void(RigidBody& body)> observer) = 0;
    };

}
