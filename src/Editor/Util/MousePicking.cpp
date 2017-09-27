#include "MousePicking.hpp"
#include <Engine/Util/Input.hpp>
#include <Engine/MainWindow.hpp>
#include <Engine/Geometry/MathFunctions.hpp>
#include <Engine/Component/Lens.hpp>
#include <glm\gtx\transform.hpp>
#include "ImGui/Theme.hpp"

#include <imgui.h>

MousePicking::MousePicking() {
    /// Fake constructor.
}

void MousePicking::CreateMousePicker(Entity * cam, glm::mat4 projection) {
    this->camera = cam;
    this->pMatrix = projection;

    glm::mat4 viewMatrix = camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition());
    this->vMatrix = viewMatrix;
}

MousePicking::~MousePicking() {
    /// Deconstructor.
}

glm::vec3 MousePicking::GetCurrentRay() {
    return this->currentRay;
}

void MousePicking::UpdateProjectionMatrix(glm::mat4 projection) {
    this->pMatrix = projection;
}

void MousePicking::Update() {

    glm::mat4 viewMatrix = camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition());
    this->vMatrix = viewMatrix;
    this->currentRay = CalculateRay();
}

glm::vec3 MousePicking::CalculateRay() {
    double mouseX = Input()->GetCursorX();
    double mouseY = Input()->GetCursorY();

    glm::vec2 NDC = GetNDC(mouseX, mouseY);
    glm::vec4 clipSpaceCoordinates = glm::vec4(NDC.x, NDC.y, -1.0f, 1.0f);
    glm::vec4 eyeSpaceCoordinates = ConvertEyeCoords(clipSpaceCoordinates);
    glm::vec3 worldRay = ConvertWorldCoords(eyeSpaceCoordinates);

    return worldRay;
}

glm::vec2 MousePicking::GetNDC(double mouseX, double mouseY) {
    double x = (2.0f*mouseX / MainWindow::GetInstance()->GetSize().x - 1.0f);
    double y = 1.0f - (2.0f*mouseY / MainWindow::GetInstance()->GetSize().y);

    return glm::vec2(x, y);
}

glm::vec4 MousePicking::ConvertEyeCoords(glm::vec4 clipSpaceCoordinates) {
    glm::mat4 invertProjection = glm::inverse(pMatrix);
    glm::vec4 eyeCoords = invertProjection * clipSpaceCoordinates;

    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicking::ConvertWorldCoords(glm::vec4 eyeCoords) {
    glm::mat4 invertedView = glm::inverse(vMatrix);
    glm::vec4 rayInWorld = invertedView * eyeCoords;
    glm::vec3 mouseRay = glm::vec3(rayInWorld.x, rayInWorld.y, rayInWorld.z);
    glm::normalize(mouseRay);

    return mouseRay;
}
