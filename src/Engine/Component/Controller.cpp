#include "Controller.hpp"
#include "Manager/Managers.hpp"
#include <Utility/Log.hpp> // TMP

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

    glm::mat4 Controller::HandleTransformation(Entity* entity) {

        glm::mat4 ctrlTransform = Managers().vrManager->GetControllerPoseMatrix(controllerID); 
        glm::vec3 ctrlRight = glm::vec3(ctrlTransform[0][0], ctrlTransform[1][0], ctrlTransform[2][0]);
        glm::vec3 ctrlUp = glm::vec3(ctrlTransform[0][1], ctrlTransform[1][1], ctrlTransform[2][1]);
        glm::vec3 ctrlForward = glm::vec3(ctrlTransform[0][2], ctrlTransform[1][2], ctrlTransform[2][2]);
        glm::vec3 ctrlPosition = glm::vec3(-ctrlTransform[3][0], -ctrlTransform[3][1], -ctrlTransform[3][2]);


        glm::mat4 ctrlOrientation = glm::mat4(
            glm::vec4(ctrlRight, 0.f),
            glm::vec4(ctrlUp, 0.f),
            glm::vec4(ctrlForward, 0.f),
            glm::vec4(0.f, 0.f, 0.f, 1.f)
        );

        //glm::mat4 hmdTranslationLocal = glm::inverse(glm::transpose(ctrlOrientation)) * ctrlTransform;
        //glm::vec3 hmdPositionLocal = glm::vec3(hmdTranslationLocal[3][0], hmdTranslationLocal[3][1], hmdTranslationLocal[3][2]);

        //glm::vec3 globalPosition = entity->GetWorldPosition() + ctrlPosition * 2.0f;
        //glm::mat4 globalTranslation = glm::translate(glm::mat4(), globalPosition);

        //ctrlOrientation = ctrlOrientation; //*entity->GetOrientation();

        //glm::mat4 ctrlTranslationLocal = glm::inverse(ctrlOrientation) * ctrlTransform;
        //glm::vec3 ctrlPositionLocal = glm::vec3(ctrlTranslationLocal[3][0], ctrlTranslationLocal[3][1], ctrlTranslationLocal[3][2]);
        //glm::vec3 ctrlPositionScaled = ctrlPositionLocal * 2.0f;
        //glm::mat4 ctrlTranslationScaled = glm::translate(glm::mat4(), ctrlPositionScaled);
        //glm::mat4 ctrlModelMatrix = ctrlTranslationScaled * ctrlOrientation * glm::scale(glm::mat4(), entity->scale);

        //ctrlModelMatrix = entity->GetLocalMatrix() * ctrlModelMatrix;
        //ctrlModelMatrix = entity->GetModelMatrix() * ctrlModelMatrix;

        glm::vec3 localPosition = ctrlPosition * 8.5f;
        glm::mat4 localTranslationMatrix = glm::translate(glm::mat4(), localPosition);
        glm::mat4 globalTranslationMatix = entity->GetModelMatrix() * (ctrlOrientation * localTranslationMatrix * glm::mat4());

        return globalTranslationMatix;
    }

    bool Controller::HandleInput(int buttonID) {
        vr::EVRButtonId vrButton = static_cast<vr::EVRButtonId>(buttonID);

        return Managers().vrManager->GetInput(vrButton);
    }
}
