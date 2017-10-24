#include "VRDevice.hpp"
#include "Manager/Managers.hpp"

namespace Component {
    VRDevice::VRDevice() {
        controllerID = 1;
    }

    VRDevice::~VRDevice() {

    }

    Json::Value VRDevice::Save() const {
        Json::Value component;

        switch (type) {
        case CONTROLLER:
            component["type"] = "controller";
            break;
        case HEADSET:
            component["type"] = "headset";
            break;
        }
        component["controllerID"] = controllerID;

        return component;
    }

    glm::mat4 VRDevice::HandleTransformation(Entity* entity) {
        return Managers().vrManager->GetHandleTransformation(controllerID, entity);
    }

    glm::mat4 VRDevice::GetHMDProjectionMatrix(vr::Hmd_Eye eye, float zNear, float zFar) const {
        return Managers().vrManager->GetHMDProjectionMatrix(eye, zNear, zFar);
    }

    bool VRDevice::HandleInput(int buttonID) {
        vr::EVRButtonId vrButton = static_cast<vr::EVRButtonId>(buttonID);

        return Managers().vrManager->GetInput(vrButton);
    }
}
