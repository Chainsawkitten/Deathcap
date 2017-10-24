#pragma once
#include "../linking.hpp"
#include "../Entity/ComponentContainer.hpp"

class SuperTrigger;

namespace Component {
    class Trigger;
}

class TriggerManager {
    friend class Hub;

public:

    TriggerManager();
    ~TriggerManager();
    TriggerManager(TriggerManager const&) = delete;
    void operator=(TriggerManager const&) = delete;

    /// Remove all killed components.
    void ClearKilledComponents();

    Component::Trigger* CreateTrigger();
    ENGINE_API const std::vector<Component::Trigger*>& GetTriggerComponents();

    ENGINE_API  void CreateTrigger(SuperTrigger* trigger);


private:

    ComponentContainer<Component::Trigger> triggerComponents;

};
