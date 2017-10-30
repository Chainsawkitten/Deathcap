#include "TriggerOnce.hpp"

<<<<<<< HEAD

=======
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
TriggerOnce::TriggerOnce() {

}

TriggerOnce::~TriggerOnce() {

}

void TriggerOnce::OnEnter() {

}

void TriggerOnce::OnLeave() {

}

<<<<<<< HEAD

=======
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
void TriggerOnce::OnRemain() {

}

std::string TriggerOnce::GetName() {
    return name;
}

void TriggerOnce::SetName(std::string value) {
    name = value;
}

std::string TriggerOnce::GetTargetFunction() {
    return targetFunction;
}

void TriggerOnce::SetTargetFunction(std::string value) {
    targetFunction = value;
}

bool TriggerOnce::GetStartActive() {
    return startActive;
}

void TriggerOnce::SetStartActive(bool value) {
    startActive = value;
}

float TriggerOnce::GetDelay() {
    return delay;
}

void TriggerOnce::SetDelay(float value) {
    delay = value;
}

Entity* TriggerOnce::GetTargetEntity() {
    return targetEntity;
}

void TriggerOnce::SetTargetEntity(Entity* value) {
    targetEntity = value;
}

Entity* TriggerOnce::GetCollidedEntity() {
    return collidedEntity;
}

void TriggerOnce::SetCollidedEntity(Entity* value) {
    collidedEntity = value;
}
