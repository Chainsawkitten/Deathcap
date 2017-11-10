#include "VRManager.hpp"

#include <Utility/Log.hpp>
#include "../Component/VRDevice.hpp"
#include "../Entity/Entity.hpp"

VRManager::VRManager() : scale(1.f) {
    // Check if VR runtime is installed.
    if (!vr::VR_IsRuntimeInstalled()) {
        vrSystem = nullptr;
        Log() << "VR runtime not installed. Playing without VR.\n";
        return;
    }
    
    // Load VR Runtime.
    vr::EVRInitError eError = vr::VRInitError_None;
    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
    if (eError != vr::VRInitError_None) {
        vrSystem = nullptr;
        Log() << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << "\n";
        return;
    }

    // Get focus.
    vr::VRCompositor()->WaitGetPoses(NULL, 0, NULL, 0);
}

VRManager::~VRManager() {
    if (vrSystem == nullptr)
        return;
    
    vr::VR_Shutdown();
    vrSystem = nullptr;
}

bool VRManager::Active() const {
    return vrSystem != nullptr;
}

void VRManager::Sync() {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return;
    }

    // Get VR device pose(s).
    vr::VRCompositor()->WaitGetPoses(tracedDevicePoseArray, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    // Convert to glm format.
    for (uint32_t nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
        if (tracedDevicePoseArray[nDevice].bPoseIsValid)
            deviceTransforms[nDevice] = ConvertMatrix(tracedDevicePoseArray[nDevice].mDeviceToAbsoluteTracking);
}

void VRManager::Update() {
    if (vrSystem == nullptr)
        return;
    
    // Update VR devices.
    for (Component::VRDevice* vrDevice : vrDevices.GetAll()) {
        if (vrDevice->IsKilled() || !vrDevice->entity->enabled)
            continue;
        
        Entity* entity = vrDevice->entity;

        // Get transformation matrix from device.
        glm::mat4 transform;
        if (vrDevice->type == Component::VRDevice::CONTROLLER) {
            transform = GetControllerPoseMatrix(vrDevice->controllerID);
        } else if (vrDevice->type == Component::VRDevice::HEADSET) {
            transform = GetHMDPoseMatrix();
        }

        glm::quat tempQuat = glm::quat(transform);

        // Update position based on device position.
        glm::vec3 position = glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
        entity->position = position * GetScale();

        entity->SetLocalOrientation(tempQuat);
    }
}

glm::vec2 VRManager::GetRecommendedRenderTargetSize() const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return glm::vec2();
    }

    std::uint32_t width, height;
    vrSystem->GetRecommendedRenderTargetSize(&width, &height);

    return glm::vec2(width, height);
}

glm::mat4 VRManager::GetHMDHeadToEyeMatrix(vr::Hmd_Eye eye) const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return glm::mat4();
    }
    return glm::inverse(ConvertMatrix(vrSystem->GetEyeToHeadTransform(eye)));
}

glm::mat4 VRManager::GetHMDProjectionMatrix(vr::Hmd_Eye eye, float zNear, float zFar) const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return glm::mat4();
    }

    return ConvertMatrix(vrSystem->GetProjectionMatrix(eye, zNear, zFar));
}

void VRManager::Submit(vr::Hmd_Eye eye, vr::Texture_t* texture) const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return;
    }

    const vr::EVRCompositorError eError = vr::VRCompositor()->Submit(eye, texture);
    if (eError != vr::VRCompositorError_None)
        Log() << "Unable to submit texture to hmd: " << eError << "\n";
}

float VRManager::GetScale() const {
    return scale;
}

void VRManager::SetScale(float scale) {
    this->scale = scale;
}

bool VRManager::GetInput(vr::EVRButtonId buttonID, int ID) {
    if (ID == 1 || ID == 2) {
        vr::ETrackedControllerRole desiredRole = static_cast<vr::ETrackedControllerRole>(ID);
        for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
            vr::ETrackedControllerRole role = vrSystem->GetControllerRoleForTrackedDeviceIndex(unDevice);
            if (role == desiredRole) {
                vr::VRControllerState_t controllerState;
                if (vrSystem->GetControllerState(unDevice, &controllerState, sizeof(controllerState))) {
                    pressedTrackedDevice[unDevice] = controllerState.ulButtonPressed == 0;
                    if (controllerState.ulButtonPressed & vr::ButtonMaskFromId(buttonID))
                        return true;
                }
            }
        }
    }

    return false;
}

Component::VRDevice* VRManager::CreateVRDevice() {
    return vrDevices.Create();
}

Component::VRDevice* VRManager::CreateVRDevice(const Json::Value& node) {
    Component::VRDevice* vrDevice = vrDevices.Create();

    // Load values from Json node.
    std::string type = node.get("type", "controller").asString();
    if (type == "controller")
        vrDevice->type = Component::VRDevice::CONTROLLER;
    else if (type == "headset")
        vrDevice->type = Component::VRDevice::HEADSET;
    
    vrDevice->controllerID = node.get("controllerID", 1).asInt();

    return vrDevice;
}

const std::vector<Component::VRDevice*>& VRManager::GetVRDevices() const {
    return vrDevices.GetAll();
}

void VRManager::ClearKilledComponents() {
    vrDevices.ClearKilled();
}

glm::mat4 VRManager::ConvertMatrix(const vr::HmdMatrix34_t& mat) {
    glm::mat4 glmMat(
        mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
        mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
        mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
        mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
    );
    return glmMat;
}

glm::mat4 VRManager::ConvertMatrix(const vr::HmdMatrix44_t& mat) {
    glm::mat4 glmMat(
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
    );
    return glmMat;
}

glm::mat4 VRManager::GetHMDPoseMatrix() const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return glm::mat4();
    }

    return deviceTransforms[vr::k_unTrackedDeviceIndex_Hmd];
}

glm::mat4 VRManager::GetControllerPoseMatrix(int controlID) const {
    if (vrSystem == nullptr) {
        Log() << "No initialized VR device.\n";
        return glm::mat4();
    }

    for (vr::TrackedDeviceIndex_t untrackedDevice = 0; untrackedDevice < vr::k_unMaxTrackedDeviceCount; untrackedDevice++) {
        // Skip current VR device if it's not connected
        if (!vrSystem->IsTrackedDeviceConnected(untrackedDevice))
            continue;
        // Skip current device if it's not a controller
        else if (vrSystem->GetTrackedDeviceClass(untrackedDevice) != vr::TrackedDeviceClass_Controller)
            continue;
        // Skip current controller if it's not in a valid position
        else if (!tracedDevicePoseArray[untrackedDevice].bPoseIsValid)
            continue;

        // Find out if current controller is the left or right one.
        vr::ETrackedControllerRole role = vrSystem->GetControllerRoleForTrackedDeviceIndex(untrackedDevice);

        // If we want to differentiate between left and right controller.
        if (role == vr::ETrackedControllerRole::TrackedControllerRole_Invalid)
            continue;
        else if (role == vr::ETrackedControllerRole::TrackedControllerRole_LeftHand && controlID == 1) {
            return deviceTransforms[untrackedDevice];
        }
        else if (role == vr::ETrackedControllerRole::TrackedControllerRole_RightHand && controlID == 2) {
            return deviceTransforms[untrackedDevice];
        }
    }

    return glm::mat4();
}


