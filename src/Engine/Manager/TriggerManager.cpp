#include "Managers.hpp"
#include "PhysicsManager.hpp"
#include "TriggerManager.hpp"
#include "../Component/Trigger.hpp"
#include "../Physics/Shape.hpp"
#include "../Trigger/SuperTrigger.hpp"
#include "../Trigger/TriggerRepeat.hpp"

TriggerManager::TriggerManager() {

}

TriggerManager::~TriggerManager() {

}

Component::Trigger* TriggerManager::CreateTrigger() {
    return triggerComponents.Create();
}

void TriggerManager::AddTriggerRepeat(Component::Trigger* trigger, std::shared_ptr<Physics::Shape> shape) {
    auto triggerVolume = Managers().physicsManager->CreateTrigger(shape);

    delete trigger->m_trigger;
    auto repeat = new TriggerRepeat;

    repeat->triggerVolume = triggerVolume;

    trigger->m_trigger = repeat;
    trigger->triggerType = Component::Trigger::REPEAT;
}

const std::vector<Component::Trigger*>& TriggerManager::GetTriggerComponents() const {
    return triggerComponents.GetAll();
}

void TriggerManager::ClearKilledComponents() {
    triggerComponents.ClearKilled();
}
