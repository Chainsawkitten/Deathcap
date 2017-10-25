#pragma once
#include <glm/glm.hpp>
#include <openvr.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SuperComponent.hpp"
#include "../Manager/VRManager.hpp"
#include "../Entity/Entity.hpp"
#include "../linking.hpp"

namespace Component {
    /// Synchronizes an Entity's transformation to that of a VR device (controller or headset).
    class VRDevice : public SuperComponent {
        public:
            /// The type of VR device.
            enum Type {
                CONTROLLER = 0, ///< Motion-tracking controller.
                HEADSET ///< Head-mounted device.
            };
            
            /// Constructor
            ENGINE_API VRDevice();

            /// Destructor
            ENGINE_API ~VRDevice();
        
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;

            /// Returns the projection matrix to use for the specified eye.
            /**
             * @param eye Which eye the function should return the eye matrix for.
             * @param zNear Distance to the near clip plane in meters.
             * @param zFar Distance to the far clip plane in meters.
             * @return The projection matrix.
             */
            glm::mat4 GetHMDProjectionMatrix(vr::Hmd_Eye eye, float zNear, float zFar) const;

            /// Handles all VR controller inputs (class for future implementations)
            /**
             * @param buttonID The id of the button to be handled.
             * @return Whether the checked button was pressed or not.
             */
            ENGINE_API bool HandleInput(int buttonID);

            /// The type of VR device being used.
            Type type = CONTROLLER;
            
            /// The controller's ID. 1 = left, 2 = right
            int controllerID;

        private:
            glm::mat4 transform;
    };
}
