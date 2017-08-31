#include "DeferredLighting.hpp"

#include "../Entity/Entity.hpp"
#include "../Component/Lens.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/PointLight.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include <Video/Culling/Frustum.hpp>
#include "../MainWindow.hpp"

#include <Video/Renderer.hpp>
#include <Video/Lighting/Light.hpp>

using namespace Video;

DeferredLighting::DeferredLighting(Renderer* renderer) {
    this->renderer = renderer;
}

void DeferredLighting::Render(World& world, const Entity* camera) {
    // Get the camera matrices.
    glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->position));
    glm::mat4 projectionMat(camera->GetComponent<Component::Lens>()->GetProjection(MainWindow::GetInstance()->GetSize()));
    glm::mat4 viewProjectionMat(projectionMat * viewMat);
    
    float cutOff;
    AxisAlignedBoundingBox aabb(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Add all directional lights.
    std::vector<Component::DirectionalLight*>& directionalLights = world.GetComponents<Component::DirectionalLight>();
    for (Component::DirectionalLight* directionalLight : directionalLights) {
        Entity* lightEntity = directionalLight->entity;
        glm::vec4 direction(glm::vec4(lightEntity->GetDirection(), 0.f));
        Light light;
        light.position = viewMat * -direction;
        light.intensities = directionalLight->color;
        light.attenuation = 1.f;
        light.ambientCoefficient = directionalLight->ambientCoefficient;
        light.coneAngle = 0.f;
        light.direction = glm::vec3(0.f, 0.f, 0.f);
        renderer->AddLight(light);
    }
    
    // Add all spot lights.
    std::vector<Component::SpotLight*>& spotLights = world.GetComponents<Component::SpotLight>();
    for (Component::SpotLight* spotLight : spotLights) {
        Entity* lightEntity = spotLight->entity;
        glm::vec4 direction(viewMat * glm::vec4(lightEntity->GetDirection(), 0.f));
        glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
        Light light;
        light.position = viewMat * (glm::vec4(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), 1.0));
        light.intensities = spotLight->color * spotLight->intensity;
        light.attenuation = spotLight->attenuation;
        light.ambientCoefficient = spotLight->ambientCoefficient;
        light.coneAngle = spotLight->coneAngle;
        light.direction = glm::vec3(direction);
        renderer->AddLight(light);
    }
    
    // At which point lights should be cut off (no longer contribute).
    cutOff = 0.0001f;
    
    // Add all point lights.
    std::vector<Component::PointLight*>& pointLights = world.GetComponents<Component::PointLight>();
    for (Component::PointLight* pointLight : pointLights) {
        Entity* lightEntity = pointLight->entity;
        float scale = sqrt((1.f / cutOff - 1.f) / pointLight->attenuation);
        glm::mat4 modelMat = glm::translate(glm::mat4(), lightEntity->position) * glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f) * scale);
        
        Frustum frustum(viewProjectionMat * modelMat);
        if (frustum.Collide(aabb)) {
            glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
            Light light;
            light.position = viewMat * (glm::vec4(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), 1.0));
            light.intensities = pointLight->color * pointLight->intensity;
            light.attenuation = pointLight->attenuation;
            light.ambientCoefficient = pointLight->ambientCoefficient;
            light.coneAngle = 180.f;
            light.direction = glm::vec3(1.f, 0.f, 0.f);
            renderer->AddLight(light);
        }
    }
    
    // Render lights.
    renderer->Light(glm::inverse(projectionMat));
}
