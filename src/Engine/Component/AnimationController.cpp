#include "AnimationController.hpp"

Component::AnimationController::AnimationController(Entity * entity) : SuperComponent(entity) {
}

Json::Value Component::AnimationController::Save() const {
    return Json::Value();
}

void Component::AnimationController::Load(const Json::Value & node) {
}

void Component::AnimationController::PlayAnimation(std::string animation) {

}
