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
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/Material.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/PointLight.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/SoundSource.hpp"
#include <Video/Geometry/Geometry3D.hpp>
#include "../Texture/TextureAsset.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Video/Culling/Frustum.hpp>
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include "../MainWindow.hpp"
#include <Video/Lighting/Light.hpp>
#include "../Hymn.hpp"
#include "Util/Profiling.hpp"

#include "Manager/VRManager.hpp"

using namespace Component;

RenderManager::RenderManager() {
    renderer = new Video::Renderer();
    
    // Render surface for main window.
    mainWindowRenderSurface = new Video::RenderSurface(MainWindow::GetInstance()->GetSize());

    // Render surface for hmd.
    hmdRenderSurface = nullptr;
    if (Managers().vrManager->Active())
        hmdRenderSurface = new Video::RenderSurface(Managers().vrManager->GetRecommendedRenderTargetSize());

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
    
    delete mainWindowRenderSurface;

    if (hmdRenderSurface != nullptr)
        delete hmdRenderSurface;

    delete renderer;
}

void RenderManager::Render(World& world, Entity* camera) {
    renderer->Clear();

    // Find camera entity.
    if (camera == nullptr) {
        std::vector<Lens*> lenses = this->GetComponents<Lens>(&world);
        for (Lens* lens : lenses)
            camera = lens->entity;
    }

    if (camera != nullptr) {
        // Render main window.
        if (mainWindowRenderSurface != nullptr) {
            const glm::vec3 position = camera->GetWorldPosition();
            const glm::mat4 orientationMat = camera->GetCameraOrientation();
            const glm::mat4 projectionMat(camera->GetComponent<Lens>()->GetProjection(mainWindowRenderSurface->GetSize()));

            Render(world, position, orientationMat, projectionMat, mainWindowRenderSurface);
        }

        //// Render hmd.
        //if (hmdRenderSurface != nullptr) {
        //    glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition()));
        //    glm::mat4 projectionMat(camera->GetComponent<Lens>()->GetProjection(hmdRenderSurface->GetSize()));
        //    glm::mat4 viewProjectionMatrix(projectionMat * viewMat);
        //    glm::vec3 up(glm::inverse(camera->GetCameraOrientation())* glm::vec4(0, 1, 0, 1));

        //    Render(world, camera, hmdRenderSurface);
        //}
    }
}

void RenderManager::RenderEditorEntities(World& world, Entity* camera, bool soundSources, bool particleEmitters, bool lightSources, bool cameras) {
    // Find camera entity.
    if (camera == nullptr) {
        std::vector<Lens*> lenses = this->GetComponents<Lens>(&world);
        for (Lens* lens : lenses) {
            camera = lens->entity;
        }
    }
    
    // Render from camera.
    if (camera != nullptr) {
        const glm::vec2 screenSize(MainWindow::GetInstance()->GetSize());
        const glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->GetWorldPosition()));
        const glm::mat4 projectionMat(camera->GetComponent<Lens>()->GetProjection(screenSize));
        const glm::mat4 viewProjectionMatrix(projectionMat * viewMat);
        const glm::vec3 up(glm::inverse(camera->GetCameraOrientation()) * glm::vec4(0, 1, 0, 1));
        
        renderer->PrepareRenderingIcons(viewProjectionMatrix, camera->GetWorldPosition(), up);
        
        // Render sound sources.
        if (soundSources) {
            for (SoundSource* soundSource : Managers().soundManager->GetComponents<SoundSource>(&world))
                renderer->RenderIcon(soundSource->entity->GetWorldPosition(), soundSourceTexture);
        }
        
        // Render particle emitters.
        if (particleEmitters) {
            for (ParticleEmitter* emitter : Managers().particleManager->GetComponents<ParticleEmitter>(&world))
                renderer->RenderIcon(emitter->entity->GetWorldPosition(), particleEmitterTexture);
        }
        
        // Render light sources.
        if (lightSources) {
            for (DirectionalLight* light : this->GetComponents<DirectionalLight>(&world))
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
            
            for (PointLight* light : this->GetComponents<PointLight>(&world))
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
            
            for (SpotLight* light : this->GetComponents<SpotLight>(&world))
                renderer->RenderIcon(light->entity->GetWorldPosition(), lightTexture);
        }
        
        // Render cameras.
        if (cameras) {
            for (Lens* lens : this->GetComponents<Lens>(&world))
                renderer->RenderIcon(lens->entity->GetWorldPosition(), cameraTexture);
        }
        
        renderer->StopRenderingIcons();
    }
}

void RenderManager::UpdateBufferSize() {
    delete mainWindowRenderSurface;

    mainWindowRenderSurface = new Video::RenderSurface(MainWindow::GetInstance()->GetSize());
}

void RenderManager::Render(World& world, const glm::vec3& position, const glm::mat4& orientationMatrix, const glm::mat4& projectionMatrix, Video::RenderSurface* renderSurface) {
    // Render from camera.
    renderer->StartRendering(renderSurface);

    // Camera matrices.
    const glm::vec3 up = glm::vec3(glm::inverse(orientationMatrix) * glm::vec4(0, 1, 0, 1));
    const glm::mat4 viewMatrix = glm::mat4(orientationMatrix * glm::translate(glm::mat4(), -position));
    const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    std::vector<Mesh*> meshes = this->GetComponents<Mesh>(&world);

    // Render static meshes.
    {
        PROFILE("Render static meshes");
        renderer->PrepareStaticMeshRendering(viewMatrix, projectionMatrix);
        for (Mesh* mesh : meshes) {
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
        LightWorld(world, viewMatrix, projectionMatrix, viewProjectionMatrix, renderSurface);
    }


    // Anti-aliasing.
    if (Hymn().filterSettings.fxaa) {
        PROFILE("Anti-aliasing(FXAA)");
        renderer->AntiAlias(renderSurface);
    }


    // Fog.
    if (Hymn().filterSettings.fog) {
        PROFILE("Fog");
        renderer->RenderFog(renderSurface, projectionMatrix, Hymn().filterSettings.fogDensity, Hymn().filterSettings.fogColor);
    }

    // Render particles.
    {
        PROFILE("Render particles");
        Managers().particleManager->UpdateBuffer(world);
        Managers().particleManager->Render(world, position, up, viewProjectionMatrix);
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

void RenderManager::LightWorld(World& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& viewProjectionMatrix, Video::RenderSurface* renderSurface) {

    float cutOff;
    Video::AxisAlignedBoundingBox aabb(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Add all directional lights.
    std::vector<Component::DirectionalLight*>& directionalLights = this->GetComponents<Component::DirectionalLight>(&world);
    for (Component::DirectionalLight* directionalLight : directionalLights) {
        if (directionalLight->IsKilled() || !directionalLight->entity->enabled)
            continue;
        
        Entity* lightEntity = directionalLight->entity;
        glm::vec4 direction(glm::vec4(lightEntity->GetDirection(), 0.f));
        Video::Light light;
        light.position = viewMatrix * -direction;
        light.intensities = directionalLight->color;
        light.attenuation = 1.f;
        light.ambientCoefficient = directionalLight->ambientCoefficient;
        light.coneAngle = 0.f;
        light.direction = glm::vec3(0.f, 0.f, 0.f);
        renderer->AddLight(light);
    }
    
    // Add all spot lights.
    std::vector<Component::SpotLight*>& spotLights = this->GetComponents<Component::SpotLight>(&world);
    for (Component::SpotLight* spotLight : spotLights) {
        if (spotLight->IsKilled() || !spotLight->entity->enabled)
            continue;
        
        Entity* lightEntity = spotLight->entity;
        glm::vec4 direction(viewMatrix * glm::vec4(lightEntity->GetDirection(), 0.f));
        glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
        Video::Light light;
        light.position = viewMatrix * (glm::vec4(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), 1.0));
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
    std::vector<Component::PointLight*>& pointLights = this->GetComponents<Component::PointLight>(&world);
    for (Component::PointLight* pointLight : pointLights) {
        if (pointLight->IsKilled() || !pointLight->entity->enabled)
            continue;
        
        Entity* lightEntity = pointLight->entity;
        float scale = sqrt((1.f / cutOff - 1.f) / pointLight->attenuation);
        glm::mat4 modelMat = glm::translate(glm::mat4(), lightEntity->GetWorldPosition()) * glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f) * scale);
        
        Video::Frustum frustum(viewProjectionMatrix * modelMat);
        if (frustum.Collide(aabb)) {
            glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
            Video::Light light;
            light.position = viewMatrix * (glm::vec4(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), 1.0));
            light.intensities = pointLight->color * pointLight->intensity;
            light.attenuation = pointLight->attenuation;
            light.ambientCoefficient = pointLight->ambientCoefficient;
            light.coneAngle = 180.f;
            light.direction = glm::vec3(1.f, 0.f, 0.f);
            renderer->AddLight(light);
        }
    }
    
    // Render lights.
    renderer->Light(glm::inverse(projectionMatrix), renderSurface);
}
