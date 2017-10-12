#pragma once
#include "SuperComponent.hpp"
#include "../Manager/VRManager.hpp"
#include "../Entity/Entity.hpp"
#include <glm/glm.hpp>
#include <openvr.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Component {
    /// Component giving VR controller functions to an Entity
    class Controller : public SuperComponent {

    public:
        /// Constructor
        Controller();

        /// Destructor
        ~Controller();            
        
        /// Save the component.
        /**
         * @return JSON value to be stored on disk.
         */
        Json::Value Save() const override;

        /// Get's the VR controller's transformation matrix
        /**
         * @return The transformation matrix.
         */
        glm::mat4 HandleTransformation(Entity* entity);

        /// Handles all VR controller inputs (class for future implementations)
        bool HandleInput(int buttonID);

        /// The controller's ID. 1 = left, 2 = right
        int controllerID;

    private:
        glm::mat4 transform;
    };
}
