#include "TriggerManager.hpp"
#include "../Component/Trigger.hpp"
#include "../Trigger/SuperTrigger.hpp"

TriggerManager::TriggerManager() {

}

TriggerManager::~TriggerManager() {

}

Component::Trigger* TriggerManager::CreateTrigger() {
    return triggerComponents.Create();
}

const std::vector<Component::Trigger*>& TriggerManager::GetTriggerComponents() const {
    return triggerComponents.GetAll();
}

void TriggerManager::ClearKilledComponents() {
    triggerComponents.ClearKilled();
}
