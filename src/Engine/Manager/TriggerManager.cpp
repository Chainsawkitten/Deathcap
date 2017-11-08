#include "Managers.hpp"
#include "PhysicsManager.hpp"
#include "TriggerManager.hpp"
#include "ScriptManager.hpp"
#include "../Component/Trigger.hpp"
#include "../Entity/Entity.hpp"
#include "../Physics/Shape.hpp"
#include "../Trigger/SuperTrigger.hpp"
#include "../Trigger/TriggerRepeat.hpp"
#include "../Util/Json.hpp"


TriggerManager::TriggerManager() {

}

TriggerManager::~TriggerManager() {

}

void TriggerManager::ProcessTriggers() {
    for (auto trigger : triggerComponents.GetAll()) {
        if (trigger->IsKilled() || !trigger->entity->enabled)
            continue;

        trigger->m_trigger->Process();
    }
}

Component::Trigger* TriggerManager::CreateTrigger() {

    auto comp = triggerComponents.Create();

    AddTriggerRepeat(comp, std::make_shared<Physics::Shape>(Physics::Shape::Sphere(1.0f)));

    return comp;
}

Component::Trigger* TriggerManager::CreateTrigger(const Json::Value& node) {

    auto comp = triggerComponents.Create();
    auto repeat = new TriggerRepeat;
    std::string name = node.get("trigger", "").asString();
    auto triggerVolume = Managers().physicsManager->CreateTrigger(std::make_shared<Physics::Shape>(Physics::Shape::Sphere(1.0f)));
    repeat->triggerVolume = triggerVolume;

    if (!name.empty()) {
        EventStruct eventstruct;

        repeat->name = node.get("triggerName", 0).asString();
        repeat->startActive = node.get("triggerActive", 0).asBool();
        repeat->delay = node.get("triggerDelay", 0).asFloat();
        repeat->cooldown = node.get("triggerCooldown", 0).asFloat();
        repeat->triggerCharges = node.get("triggerCharges", 0).asInt();
        repeat->targetFunction.push_back(node.get("triggerFunction", 0).asString());

        repeat->collidedEntityUID = node.get("triggerCollidedEntity", 0).asInt();
        repeat->targetEntityUID = node.get("triggerTargetEntity", 0).asInt();
        repeat->owningEntityUID = node.get("triggerOwner", 0).asInt();

        eventstruct.m_eventID = node.get("triggerEventStruct_EventID", 0).asInt();
        eventstruct.m_shapeID = node.get("triggerEventStruct_ShapeID", 0).asInt();
        eventstruct.m_targetID = node.get("triggerEventStruct_TargetID", 0).asInt();
        eventstruct.m_scriptID = node.get("triggerEventStruct_ScriptID", 0).asInt();
        eventstruct.check[0] = node.get("triggerEventStruct_Check_0", 0).asInt();
        eventstruct.check[1] = node.get("triggerEventStruct_Check_1", 0).asInt();
        eventstruct.check[2] = node.get("triggerEventStruct_Check_2", 0).asInt();
        eventstruct.check[3] = node.get("triggerEventStruct_Check_3", 0).asInt();

        repeat->eventVector.push_back(eventstruct);

        repeat->triggered = node.get("triggerTriggered", 0).asBool();

        repeat->triggerVolume = triggerVolume;

        std::string test = node.toStyledString();

    }

    comp->m_trigger = repeat;

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

void TriggerManager::SynchronizeTriggers() {

    for (auto trigger : triggerComponents.GetAll()) {
        if (trigger->IsKilled() || !trigger->entity->enabled)
            continue;

        trigger->m_trigger->Update();

    }
}

void TriggerManager::ClearKilledComponents() {
    triggerComponents.ClearKilled();
}

void TriggerManager::InitiateUID() {
    for (auto trigger : triggerComponents.GetAll()) {
        if (trigger->IsKilled() || !trigger->entity->enabled)
            continue;

        trigger->m_trigger->InitTriggerUID();

    }
}