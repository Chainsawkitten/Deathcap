#include "TriggerRepeat.hpp"

#include "../Component/RigidBody.hpp"
#include "../Component/Shape.hpp"
#include "../Entity/Entity.hpp"
#include "../Manager/Managers.hpp"
#include "../Manager/PhysicsManager.hpp"
#include "../Manager/ScriptManager.hpp"
#include "../Physics/Shape.hpp"
#include "../Hymn.hpp"


TriggerRepeat::TriggerRepeat() {

}

TriggerRepeat::~TriggerRepeat() {

}

// ADD SUPPORT FOR VECTOR
void TriggerRepeat::OnEnter() {
    if (!collidedEntity.empty() && collidedEntity.front()->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity.front()->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerEnter(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
}

// ADD SUPPORT FOR VECTOR
void TriggerRepeat::OnRetain() {
    if (!collidedEntity.empty() && collidedEntity.front()->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity.front()->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerRetain(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
}

// ADD SUPPORT FOR VECTOR
void TriggerRepeat::OnLeave() {

    if (!collidedEntity.empty() && collidedEntity.front()->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity.front()->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerLeave(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
}

const std::string& TriggerRepeat::GetName() const {
    return name;
}

void TriggerRepeat::SetName(std::string& value) {
    name = value;
}

bool TriggerRepeat::GetStartActive() {
    return startActive;
}

void TriggerRepeat::SetStartActive(bool value) {
    startActive = value;
}

float TriggerRepeat::GetDelay() {
    return delay;
}

void TriggerRepeat::SetDelay(float value) {
    delay = value;
}

float TriggerRepeat::GetCooldown() {
    return cooldown;
}

void TriggerRepeat::SetCooldown(float value) {
    cooldown = value;
}

int TriggerRepeat::GetTriggerCharges() {
    return triggerCharges;
}

void TriggerRepeat::SetTriggerCharges(int value) {
    triggerCharges = value;
}

void TriggerRepeat::HandleTriggerEvent() {
    triggered = true;
}

std::vector<triggerEvent::EventStruct> *TriggerRepeat::GetEventVector() {
    return &eventVector;
}

Entity* TriggerRepeat::GetOwningEntity() {
    return owningEntity;
}

void TriggerRepeat::SetOwningEntity(Entity* value) {
    owningEntity = value;
}

void TriggerRepeat::InitTriggerUID() {

    for (int i = 0; i < eventVector.size(); i++) {

        if (Hymn().GetEntityByGUID(eventVector.at(i).targetEntityUID) != nullptr)
            eventVector.at(i).targetEntity = Hymn().GetEntityByGUID(eventVector.at(i).targetEntityUID);
        if (Hymn().GetEntityByGUID(eventVector.at(i).targetEntityUID) != nullptr)
            eventVector.at(i).collidedEntity = Hymn().GetEntityByGUID(eventVector.at(i).collidedEntityUID);
        if (Hymn().GetEntityByGUID(eventVector.at(i).targetEntityUID) != nullptr)
            owningEntity = Hymn().GetEntityByGUID(owningEntityUID);
    }

}

void TriggerRepeat::Process() {
    // ADD SUPPORT FOR VECTOR
    if (triggered) {
        Managers().scriptManager->ExecuteScriptMethod(targetEntity.front(), targetFunction.front());
        triggered = false;
    }
}

void TriggerRepeat::Update() {

    if (owningEntity != nullptr) {
        Managers().physicsManager->SetPosition(triggerVolume, owningEntity->GetWorldPosition());

        if (owningEntity->GetComponent<Component::Shape>() != nullptr) {
            Managers().physicsManager->SetShape(triggerVolume, owningEntity->GetComponent<Component::Shape>()->GetShape());
        }
    }
}

bool TriggerRepeat::CheckVector(const triggerEvent::EventStruct& value) const {

    if (value.targetFunction.empty())
        return false;
    else if (value.targetEntity == nullptr)
        return false;
    else if (value.collidedEntity == nullptr)
        return false;
    else
        return true;
}

Json::Value TriggerRepeat::Save() {
    Json::Value component;

    component["trigger"] = "A Trigger";
    component["triggerName"] = name;
    component["triggerActive"] = startActive;
    component["triggerDelay"] = delay;
    component["triggerCooldown"] = cooldown;
    component["triggerCharges"] = triggerCharges;
    component["triggerTriggered"] = triggered;
    component["triggerVectorSize"] = eventVector.size();

    if (owningEntity != nullptr)
        component["triggerOwner"] = owningEntity->GetUniqueIdentifier();

    for (int i = 0; i < eventVector.size(); i++) {        

        component["triggerFunction"] = eventVector.at(i).targetFunction;
        component["triggerTargetEntity"] = eventVector.at(i).targetEntity->GetUniqueIdentifier();
        component["triggerCollidedEntity"] = eventVector.at(i).collidedEntity->GetUniqueIdentifier();
        component["triggerEventStruct_EventID"] = eventVector.at(i).eventID;
        component["triggerEventStruct_ShapeID"] = eventVector.at(i).shapeID;
        component["triggerEventStruct_TargetID"] = eventVector.at(i).targetID;
        component["triggerEventStruct_ScriptID"] = eventVector.at(i).scriptID;

        if (eventVector.at(i).triggerCheck && eventVector.at(i).eventID == triggerEvent::OnEnter)
            OnEnter();
        else if (eventVector.at(i).triggerCheck && eventVector.at(i).eventID == triggerEvent::OnRemain)
            OnRetain();
        else if (eventVector.at(i).triggerCheck && eventVector.at(i).eventID == triggerEvent::OnLeave)
            OnLeave();
    }
    return component;
}
