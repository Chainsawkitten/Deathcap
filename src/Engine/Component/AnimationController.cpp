#include "AnimationController.hpp"

using namespace Component;

AnimationController::AnimationController(Entity * entity) : SuperComponent(entity) {
}

Json::Value AnimationController::Save() const {
    return Json::Value();
}

void AnimationController::Load(const Json::Value & node) {
}