#include "RenderManager.hpp"

#include <Video/Renderer.hpp>
#include <Video/RenderSurface.hpp>
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include "ParticleManager.hpp"
#include "SoundManager.hpp"
#include "Light.png.hpp"
#include "ParticleEmitter.png.hpp"
#include "SoundSource.png.hpp"
#include "Camera.png.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Animation.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/Material.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/PointLight.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/SoundSource.hpp"
#include <Video/Geometry/Geometry3D.hpp>
#include "../Geometry/Skeleton.hpp"
#include "../Texture/TextureAsset.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Video/Culling/Frustum.hpp>
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include "../MainWindow.hpp"
#include <Video/Lighting/Light.hpp>
#include "../Hymn.hpp"
#include "Util/Profiling.hpp"

#include "Manager/Managers.hpp"

using namespace Component;

RenderManager::RenderManager() {
    renderer = new Video::Renderer();
    
    // Render surface for main window.
    renderSurface = new Video::RenderSurface(MainWindow::GetInstance()->GetSize());

    // Init textures.
    particleEmitterTexture = Managers().resourceManager->CreateTexture2D(PARTICLEEMITTER_PNG, PARTICLEEMITTER_PNG_LENGTH);
    lightTexture = Managers().resourceManager->CreateTexture2D(LIGHT_PNG, LIGHT_PNG_LENGTH);
    soundSourceTexture = Managers().resourceManager->CreateTexture2D(SOUNDSOURCE_PNG, SOUNDSOURCE_PNG_LENGTH);
    cameraTexture = Managers().resourceManager->CreateTexture2D(CAMERA_PNG, CAMERA_PNG_LENGTH);
}

RenderManager::~RenderManager() {
    Managers().resourceManager->FreeTexture2D(particleEmitterTexture);
    Managers().resourceManager->FreeTexture2D(lightTexture);
    Managers().resourceManager->FreeTexture2D(soundSourceTexture);
    Managers().resourceManager->FreeTexture2D(cameraTexture);
    
    delete renderSurface;

    delete renderer;
}

void RenderManager::Render(World& world, Entity* camera) {
    renderer->Clear();
    
    // Find camera entity.
    if (camera == nullptr) {
        for (Lens* lens : lenses.GetAll()) {
            camera = lens->entity;
        }
    }
    
    // Render from camera.
    if (camera != nullptr) {
        renderer->StartRendering(renderSurface);
        
        // Camera matrices.
        const glm::mat4 viewMatrix = camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition());
        const glm::mat4 projectionMatrix = camera->GetComponent<Lens>()->GetProjection(renderSurface->GetSize());
        
        const std::vector<Mesh*>& meshComponents = meshes.GetAll();
        
        // Render static meshes.
        {
            PROFILE("Render static meshes");
            renderer->PrepareStaticMeshRendering(viewMatrix, projectionMatrix);
            for (Mesh* mesh : meshComponents) {
                if (mesh->IsKilled() || !mesh->entity->enabled)
                    continue;
                
                if (mesh->geometry != nullptr && mesh->geometry->GetType() == Video::Geometry::Geometry3D::STATIC) {
                    Entity* entity = mesh->entity;
                    Material* material = entity->GetComponent<Material>();
                    if (material != nullptr) {
                        renderer->RenderStaticMesh(mesh->geometry, material->albedo->GetTexture(), material->normal->GetTexture(), material->metallic->GetTexture(), material->roughness->GetTexture(), entity->GetModelMatrix());
                    }
                }
            }
        }

        /// @todo Render skinned meshes.
        
        // Light the world.
        {
            PROFILE("Light the world");
            LightWorld(camera, renderSurface);
        }

        
        // Anti-aliasing.
        if (Hymn().filterSettings.fxaa) {
            PROFILE("Anti-aliasing(FXAA)");
            renderer->AntiAlias(renderSurface);
        }

        
        // Fog.
        if (Hymn().filterSettings.fog) {
            PROFILE("Fog");
            renderer->RenderFog(renderSurface, camera->GetComponent<Component::Lens>()->GetProjection(renderSurface->GetSize()), Hymn().filterSettings.fogDensity, Hymn().filterSettings.fogColor);
        }
            
        // Render particles.
        {
            PROFILE("Render particles");
            Managers().particleManager->UpdateBuffer(world);
            Managers().particleManager->Render(world, camera);
        }
       
        
        // Glow.
        if (Hymn().filterSettings.glow) {
            PROFILE("Glow");
            renderer->ApplyGlow(renderSurface, Hymn().filterSettings.glowBlurAmount);
        }
        
        // Color.
        if (Hymn().filterSettings.color) {
            PROFILE("Color");
            renderer->ApplyColorFilter(renderSurface, Hymn().filterSettings.colorColor);
        }
        
        // Render to back buffer.
        { PROFILE("Render to back buffer");
            renderer->DisplayResults(renderSurface, true);
        }
    }
}

void RenderManager::RenderEditorEntities(World& world, Entity* camera, bool soundSources, bool particleEmitters, bool lightSources, bool cameras) {
    // Find camera entity.
    if (camera == nullptr) {
        for (Lens* lens : lenses.GetAll()) {
            camera = lens->entity;
        }
    }
    
    // Render from camera.
    if (camera != nullptr) {
        glm::vec2 screenSize(MainWindow::GetInstance()->GetSize());
        glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition()));
        glm::mat4 projectionMat(camera->GetComponent<Lens>()->GetProjection(screenSize));
        glm::mat4 viewProjectionMatrix(projectionMat * viewMat);
        glm::vec3 up(glm::inverse(camera->GetCameraOrientation())* glm::vec4(0, 1, 0, 1));
        
        renderer->PrepareRenderingIcons(viewProjectionMatrix, camera->GetWorldPosition(), up);
        
        // Render sound sources.
        if (soundSources) {
            for (SoundSource* soundSource : Managers().soundManager->GetSoundSources())
                renderer->RenderIcon(soundSource->entity->GetWorldPosition(), soundSourceTexture);
        }
        
        // Render particle emitters.
        if (particleEmitters) {
            for (ParticleEmitter* emitter : Managers().particleManager->GetParticleEmitters())
                renderer->RenderIcon(emitter->entity->GetWorldPosition(), particleEmitterTexture);
        }
        
        // Render light sources.
        if (lightSources) {
            for (DirectionalLight* light : directionalLights.GetAll())
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
            
            for (PointLight* light : pointLights.GetAll())
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
            
            for (SpotLight* light : spotLights.GetAll())
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
        }
        
        // Render cameras.
        if (cameras) {
            for (Lens* lens : lenses.GetAll())
                renderer->RenderIcon(lens->entity->GetWorldPosition(), cameraTexture);
        }
        
        renderer->StopRenderingIcons();
    }
}

void RenderManager::UpdateBufferSize() {
    delete renderSurface;

    renderSurface = new Video::RenderSurface(MainWindow::GetInstance()->GetSize());
}

void RenderManager::LightWorld(const Entity* camera, Video::RenderSurface* renderSurface) {
    // Get the camera matrices.
    glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition()));
    glm::mat4 projectionMat(camera->GetComponent<Component::Lens>()->GetProjection(MainWindow::GetInstance()->GetSize()));
    glm::mat4 viewProjectionMat(projectionMat * viewMat);
    
    float cutOff;
    Video::AxisAlignedBoundingBox aabb(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Add all directional lights.
    for (Component::DirectionalLight* directionalLight : directionalLights.GetAll()) {
        if (directionalLight->IsKilled() || !directionalLight->entity->enabled)
            continue;
        
        Entity* lightEntity = directionalLight->entity;
        glm::vec4 direction(glm::vec4(lightEntity->GetDirection(), 0.f));
        Video::Light light;
        light.position = viewMat * -direction;
        light.intensities = directionalLight->color;
        light.attenuation = 1.f;
        light.ambientCoefficient = directionalLight->ambientCoefficient;
        light.coneAngle = 0.f;
        light.direction = glm::vec3(0.f, 0.f, 0.f);
        renderer->AddLight(light);
    }
    
    // Add all spot lights.
    for (Component::SpotLight* spotLight : spotLights.GetAll()) {
        if (spotLight->IsKilled() || !spotLight->entity->enabled)
            continue;
        
        Entity* lightEntity = spotLight->entity;
        glm::vec4 direction(viewMat * glm::vec4(lightEntity->GetDirection(), 0.f));
        glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
        Video::Light light;
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
    for (Component::PointLight* pointLight : pointLights.GetAll()) {
        if (pointLight->IsKilled() || !pointLight->entity->enabled)
            continue;
        
        Entity* lightEntity = pointLight->entity;
        float scale = sqrt((1.f / cutOff - 1.f) / pointLight->attenuation);
        glm::mat4 modelMat = glm::translate(glm::mat4(), lightEntity->GetWorldPosition()) * glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f) * scale);
        
        Video::Frustum frustum(viewProjectionMat * modelMat);
        if (frustum.Collide(aabb)) {
            glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
            Video::Light light;
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
    renderer->Light(glm::inverse(projectionMat), renderSurface);
}

Component::Animation* RenderManager::CreateAnimation() {
    return animations.Create();
}

const std::vector<Component::Animation*>& RenderManager::GetAnimations() const {
    return animations.GetAll();
}

Component::DirectionalLight* RenderManager::CreateDirectionalLight() {
    return directionalLights.Create();
}

const std::vector<Component::DirectionalLight*>& RenderManager::GetDirectionalLights() const {
    return directionalLights.GetAll();
}

Component::Lens* RenderManager::CreateLens() {
    return lenses.Create();
}

const std::vector<Component::Lens*>& RenderManager::GetLenses() const {
    return lenses.GetAll();
}

Component::Material* RenderManager::CreateMaterial() {
    return materials.Create();
}

const std::vector<Component::Material*>& RenderManager::GetMaterials() const {
    return materials.GetAll();
}

Component::Mesh* RenderManager::CreateMesh() {
    return meshes.Create();
}

const std::vector<Component::Mesh*>& RenderManager::GetMeshes() const {
    return meshes.GetAll();
}

Component::PointLight* RenderManager::CreatePointLight() {
    return pointLights.Create();
}

const std::vector<Component::PointLight*>& RenderManager::GetPointLights() const {
    return pointLights.GetAll();
}

Component::SpotLight* RenderManager::CreateSpotLight() {
    return spotLights.Create();
}

const std::vector<Component::SpotLight*>& RenderManager::GetSpotLights() const {
    return spotLights.GetAll();
}

void RenderManager::ClearKilledComponents() {
    animations.ClearKilled();
    directionalLights.ClearKilled();
    lenses.ClearKilled();
    materials.ClearKilled();
    meshes.ClearKilled();
    pointLights.ClearKilled();
    spotLights.ClearKilled();
}
