#include "Managers.hpp"
#include "PhysicsManager.hpp"
#include "TriggerManager.hpp"
#include "../Component/Trigger.hpp"
#include "../Physics/Shape.hpp"
#include "../Trigger/SuperTrigger.hpp"
#include "../Trigger/TriggerRepeat.hpp"
#include "../Util/Json.hpp"

TriggerManager::TriggerManager() {

}

TriggerManager::~TriggerManager() {

}

Component::Trigger* TriggerManager::CreateTrigger() {

    auto comp = triggerComponents.Create();

    AddTriggerRepeat(comp, std::make_shared<Physics::Shape>(Physics::Shape::Sphere(1.0f)));

    return comp;
}

Component::Trigger* TriggerManager::CreateTrigger(const Json::Value& node) {
    auto comp = triggerComponents.Create();

    AddTriggerRepeat(comp, std::make_shared<Physics::Shape>(Physics::Shape::Sphere(1.0f)));

    return comp;
}

void TriggerManager::AddTriggerRepeat(Component::Trigger* trigger, std::shared_ptr<Physics::Shape> shape) {
    
    auto triggerVolume = Managers().physicsManager->CreateTrigger(shape);

    delete trigger->m_trigger;
    auto repeat = new TriggerRepeat;

    repeat->triggerVolume = triggerVolume;

    trigger->m_trigger = repeat;
    trigger->triggerType = Component::Trigger::REPEAT;
}

TriggerRepeat* TriggerManager::GetTriggerRepeat(Component::Trigger& trigger) {
    return dynamic_cast<TriggerRepeat*>(trigger.m_trigger);
}

const std::vector<Component::Trigger*>& TriggerManager::GetTriggerComponents() const {
    return triggerComponents.GetAll();
}

void TriggerManager::ClearKilledComponents() {
    triggerComponents.ClearKilled();
}