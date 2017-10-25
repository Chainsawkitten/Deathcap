#pragma once

#include <memory>
#include "../Entity/ComponentContainer.hpp"
#include "../linking.hpp"

class SuperTrigger;

namespace Component {
    class Trigger;
}

namespace Physics {
    class Shape;
}

class TriggerManager {
    friend class Hub;

    public:
        /// Create a trigger component.
        /**
         * @return The created component.
         */
        ENGINE_API Component::Trigger* CreateTrigger();

        /// Add a repeating trigger to the component.
        /**
         * @param trigger Component to add trigger to.
         * @param shape The physical shape of the trigger volume.
         */
        ENGINE_API void AddTriggerRepeat(Component::Trigger* trigger, std::shared_ptr<Physics::Shape> shape);

        /// Get all trigger components.
        /**
         * @return All trigger components.
         */
        ENGINE_API const std::vector<Component::Trigger*>& GetTriggerComponents() const;

        /// Remove all killed components.
        void ClearKilledComponents();

    private:
        TriggerManager();
        ~TriggerManager();
        TriggerManager(const TriggerManager&) = delete;
        void operator=(const TriggerManager&) = delete;

        ComponentContainer<Component::Trigger> triggerComponents;
};
