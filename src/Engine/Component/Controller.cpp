#include "Controller.hpp"
#include "Manager/Managers.hpp"

namespace Component {
    Controller::Controller() {
        controllerID = 1;
    }

    Controller::~Controller() {

    }

    Json::Value Controller::Save() const {
        Json::Value component;

        component["controllerID"] = controllerID;

        return component;
    }

    glm::mat4 *Controller::HandleTransformation() {
        transform = Managers().vrManager->GetControllerPoseMatrix(controllerID);
        return &transform;
    }

    bool Controller::HandleInput(int buttonID) {
        vr::EVRButtonId vrButton = static_cast<vr::EVRButtonId>(buttonID);

        return Managers().vrManager->GetInput(vrButton);
    }
}
