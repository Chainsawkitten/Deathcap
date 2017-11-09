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

Entity* GetEntity(int GUID) {

    const std::vector<Entity*> entities = Hymn().world.GetEntities();
    for (int i = 0; i < entities.size(); i++) {

        if (entities[i]->GetUniqueIdentifier() == GUID) {

            return entities[i];
        }
    }

    return nullptr;
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

std::string TriggerRepeat::GetName() {
    return name;
}

void TriggerRepeat::SetName(std::string value) {
    name = value;
}

std::vector<std::string> *TriggerRepeat::GetTargetFunction() {
    return &targetFunction;
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

std::vector<Entity*> *TriggerRepeat::GetTargetEntity() {
    return &targetEntity;
}

std::vector<Entity*> *TriggerRepeat::GetCollidedEntity() {
    return &collidedEntity;
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

    if (GetEntity(targetEntityUID) != nullptr)
        targetEntity.push_back(GetEntity(targetEntityUID));
    if (GetEntity(collidedEntityUID) != nullptr)
        collidedEntity.push_back(GetEntity(collidedEntityUID));
    if (GetEntity(owningEntityUID) != nullptr)
        owningEntity = GetEntity(owningEntityUID);

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

Json::Value TriggerRepeat::Save() {
    Json::Value component;

    component["trigger"] = "A Trigger";
    component["triggerName"] = name;
    component["triggerActive"] = startActive;
    component["triggerDelay"] = delay;
    component["triggerCooldown"] = cooldown;
    component["triggerCharges"] = triggerCharges;

    if (!targetFunction.empty())
        component["triggerFunction"] = targetFunction.front(); // ADD SUPPORT FOR VECTOR
    if (!targetEntity.empty())
        component["triggerTargetEntity"] = targetEntity.front()->GetUniqueIdentifier(); // ADD SUPPORT FOR VECTOR
    if (!collidedEntity.empty())
        component["triggerCollidedEntity"] = collidedEntity.front()->GetUniqueIdentifier(); // ADD SUPPORT FOR VECTOR

    if (!eventVector.empty()) {
        component["triggerEventStruct_EventID"] = eventVector.front().m_eventID; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_ShapeID"] = eventVector.front().m_shapeID; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_TargetID"] = eventVector.front().m_targetID; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_ScriptID"] = eventVector.front().m_scriptID; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_Check_0"] = eventVector.front().check[0]; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_Check_1"] = eventVector.front().check[1]; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_Check_2"] = eventVector.front().check[2]; // ADD SUPPORT FOR VECTOR
        component["triggerEventStruct_Check_3"] = eventVector.front().check[3]; // ADD SUPPORT FOR VECTOR
    }

    if (GetEventVector()->front().check[0] == true &&
        GetEventVector()->front().check[1] == true &&
        GetEventVector()->front().check[2] == true &&
        GetEventVector()->front().check[3] == true && GetEventVector()->front().m_eventID == 0) {
        OnEnter();
    } else if (GetEventVector()->front().check[0] == true &&
        GetEventVector()->front().check[1] == true &&
        GetEventVector()->front().check[2] == true &&
        GetEventVector()->front().check[3] == true && GetEventVector()->front().m_eventID == 1) {
        OnRetain();
    } else if (GetEventVector()->front().check[0] == true &&
        GetEventVector()->front().check[1] == true &&
        GetEventVector()->front().check[2] == true &&
        GetEventVector()->front().check[3] == true && GetEventVector()->front().m_eventID == 2) {
        OnLeave();
    }

    component["triggerTriggered"] = triggered;

    if (owningEntity != nullptr)
        component["triggerOwner"] = owningEntity->GetUniqueIdentifier();

    return component;

}
