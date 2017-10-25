#pragma once

#include <glm/glm.hpp>
#include <openvr.h>
#include "../Entity/ComponentContainer.hpp"
#include <Engine/Component/VRDevice.hpp>
#include "../linking.hpp"

namespace Component {
    class VRDevice;
}

namespace Json {
    class Value;
}

/// Handles communication with VR devices using OpenVR.
class VRManager {
    friend class Hub;
    
    public:
        /// Check if the VRManger is active.
        /**
         * @return Whether hmd is present or not.
         */
        ENGINE_API bool Active() const;

        /// Sync VR device pose(s).
        ENGINE_API void Sync();
        
        /// Update VR device components.
        void Update();

        /// Recommended render target size to minimize pixel stretching.
        /**
         * @return Recommended size (in pixels).
         */
        ENGINE_API glm::vec2 GetRecommendedRenderTargetSize() const;

        /// Returns the transform between the view space and eye space.
        /**
         * @param eye Which eye the function should return the eye matrix for.
         * @return The eye matrix.
         */
        ENGINE_API glm::mat4 GetHMDHeadToEyeMatrix(vr::Hmd_Eye eye) const;

        /// Returns the projection matrix to use for the specified eye.
        /**
         * @param eye Which eye the function should return the eye matrix for.
         * @param zNear Distance to the near clip plane in meters.
         * @param zFar Distance to the far clip plane in meters.
         * @return The projection matrix.
         */
        ENGINE_API glm::mat4 GetHMDProjectionMatrix(vr::Hmd_Eye eye, float zNear, float zFar) const;

        /// Submit a texture to an eye.
        /**
         * @param eye Which eye to submit texture.
         * @param texture Texture to submit.
         */
        ENGINE_API void Submit(vr::Hmd_Eye eye, vr::Texture_t* texture) const;

        /// Get scale of player.
        /**
         * @return Scale to get.
         */
        ENGINE_API float GetScale() const;

        /// Set scale of player.
        /**
         * @param scale Scale to set.
         */
        ENGINE_API void SetScale(float scale);

        /// Gets if certain button is pressed (Only trigger for now)
        /**
         * @param buttonID the button we want to check is pressed.
         * @return whether certain button is pressed or not. 
         */
        ENGINE_API bool GetInput(vr::EVRButtonId buttonID);
        
        /// Create VR device component.
        /**
         * @return The created component.
         */
        ENGINE_API Component::VRDevice* CreateVRDevice();

        /// Create VR device component.
        /**
         * @param node Json node to load the component from
         * @return The created component.
         */
        ENGINE_API Component::VRDevice* CreateVRDevice(const Json::Value& node);

        /// Get all VR device components.
        /**
         * @return All VR device components.
         */
        ENGINE_API const std::vector<Component::VRDevice*>& GetVRDevices() const;
        
        /// Remove all killed components.
        ENGINE_API void ClearKilledComponents();
        
    private:
        VRManager();
        ~VRManager();
        VRManager(VRManager const&) = delete;
        void operator=(VRManager const&) = delete;

        static glm::mat4 ConvertMatrix(const vr::HmdMatrix34_t& mat);
        static glm::mat4 ConvertMatrix(const vr::HmdMatrix44_t& mat);
        
        // Returns the transform for the HMD.
        glm::mat4 GetHMDPoseMatrix() const;
        
        // Returns the transform for the controllers
        // controlID determines which controller to get (1 = left, 2 = right)
        glm::mat4 GetControllerPoseMatrix(int controlID) const;

        float scale;
        vr::IVRSystem* vrSystem;
        vr::TrackedDevicePose_t tracedDevicePoseArray[vr::k_unMaxTrackedDeviceCount];
        glm::mat4 deviceTransforms[vr::k_unMaxTrackedDeviceCount];

        bool pressedTrackedDevice[vr::k_unMaxTrackedDeviceCount];

        ComponentContainer<Component::VRDevice> vrDevices;
};
