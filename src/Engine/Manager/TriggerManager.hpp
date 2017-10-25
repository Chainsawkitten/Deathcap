#pragma once

#include "../Entity/ComponentContainer.hpp"
#include "../linking.hpp"

class SuperTrigger;

namespace Component {
    class Trigger;
}

class TriggerManager {
    friend class Hub;

    public:
        /// Create a trigger component.
        /**
         * @return The created component.
         */
        ENGINE_API Component::Trigger* CreateTrigger();

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
