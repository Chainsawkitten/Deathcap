#include "TriggerRepeat.hpp"

<<<<<<< HEAD
=======
#include "../Component/RigidBody.hpp"
#include "../Entity/Entity.hpp"
#include "../Manager/Managers.hpp"
#include "../Manager/PhysicsManager.hpp"
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5

TriggerRepeat::TriggerRepeat() {

}

TriggerRepeat::~TriggerRepeat() {

}

void TriggerRepeat::OnEnter() {
<<<<<<< HEAD
    // TODO
}

void TriggerRepeat::OnLeave() {
    // TODO
}


void TriggerRepeat::OnRemain() {
    // TODO
=======
    if (collidedEntity && collidedEntity->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerEnter(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
}

void TriggerRepeat::OnRetain() {
    if (collidedEntity && collidedEntity->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerRetain(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
}

void TriggerRepeat::OnLeave() {
    if (collidedEntity && collidedEntity->GetComponent<Component::RigidBody>()) {
        Component::RigidBody* rigidBodyComp = collidedEntity->GetComponent<Component::RigidBody>();
        Managers().physicsManager->ForgetTriggerEnter(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerRetain(triggerVolume, rigidBodyComp);
        Managers().physicsManager->ForgetTriggerLeave(triggerVolume, rigidBodyComp);
        Managers().physicsManager->OnTriggerLeave(triggerVolume, rigidBodyComp, std::bind(&TriggerRepeat::HandleTriggerEvent, this));
    }
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
}

std::string TriggerRepeat::GetName() {
    return name;
}

void TriggerRepeat::SetName(std::string value) {
    name = value;
}

std::string TriggerRepeat::GetTargetFunction() {
    return targetFunction;
}

void TriggerRepeat::SetTargetFunction(std::string value) {
    targetFunction = value;
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

float TriggerRepeat::GetTriggerCharges() {
    return triggerCharges;
}

void TriggerRepeat::SetTriggerCharges(float value) {
    triggerCharges = value;
}

Entity* TriggerRepeat::GetTargetEntity() {
    return targetEntity;
}

void TriggerRepeat::SetTargetEntity(Entity* value) {
    targetEntity = value;
}

Entity* TriggerRepeat::GetCollidedEntity() {
    return collidedEntity;
}

void TriggerRepeat::SetCollidedEntity(Entity* value) {
    collidedEntity = value;
}
<<<<<<< HEAD
=======

void TriggerRepeat::HandleTriggerEvent() {
    triggered = true;
}
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
