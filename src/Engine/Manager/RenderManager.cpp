#include "RenderManager.hpp"

#include <Video/Buffer/FrameBuffer.hpp>
#include <Video/Renderer.hpp>
#include <Video/RenderSurface.hpp>
#include <Video/Buffer/ReadWriteTexture.hpp>
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include "ParticleManager.hpp"
#include "SoundManager.hpp"
#include "PhysicsManager.hpp"
#include "DebugDrawingManager.hpp"
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
#include "../Component/Shape.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/SoundSource.hpp"
#include "../Component/VRDevice.hpp"
#include "../Physics/Shape.hpp"
#include <Video/Geometry/Geometry3D.hpp>
#include "../Texture/TextureAsset.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Video/Culling/Frustum.hpp>
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include "../MainWindow.hpp"
#include <Video/Lighting/Light.hpp>
#include "../Hymn.hpp"
#include "Util/Profiling.hpp"
#include "Util/Json.hpp"
#include "Util/GPUProfiling.hpp"
#include <Utility/Log.hpp>

#include "Manager/VRManager.hpp"
#include <glm/gtc/quaternion.hpp>

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

void RenderManager::Render(World& world, bool soundSources, bool particleEmitters, bool lightSources, bool cameras, bool physics, Entity* camera) {
    // Find camera entity.
    if (camera == nullptr) {
        for (Lens* lens : lenses.GetAll())
            camera = lens->entity;
    }

    if (camera != nullptr) {
        // Set image processing variables.
        renderer->SetGamma(Hymn().filterSettings.gamma);
        renderer->SetFogApply(Hymn().filterSettings.fogApply);
        renderer->SetFogDensity(Hymn().filterSettings.fogDensity);
        renderer->SetFogColor(Hymn().filterSettings.fogColor);
        renderer->SetColorFilterApply(Hymn().filterSettings.colorFilterApply);
        renderer->SetColorFilterColor(Hymn().filterSettings.colorFilterColor);
        renderer->SetDitherApply(Hymn().filterSettings.ditherApply);
        const bool fxaa = Hymn().filterSettings.fxaa;

        // Render main window.
        const glm::vec2 windowSize = MainWindow::GetInstance()->GetSize();
        if (mainWindowRenderSurface != nullptr && windowSize.x > 0 && windowSize.y > 0) {
            { PROFILE("Render main window");
            { GPUPROFILE("Render main window", Video::Query::Type::TIME_ELAPSED);
                const glm::mat4 projectionMatrix = camera->GetComponent<Lens>()->GetProjection(mainWindowRenderSurface->GetSize());
                const glm::mat4 viewMatrix = glm::inverse(camera->GetModelMatrix());
                const glm::vec3 position = camera->GetWorldPosition();
                const glm::vec3 up(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

                { PROFILE("Render world entities");
                { GPUPROFILE("Render world entities", Video::Query::Type::TIME_ELAPSED);
                    RenderWorldEntities(world, viewMatrix, projectionMatrix, mainWindowRenderSurface);
                }
                }

                { PROFILE("Render debug entities");
                { GPUPROFILE("Render debug entities", Video::Query::Type::TIME_ELAPSED);
                    Managers().debugDrawingManager->Render(viewMatrix, projectionMatrix, mainWindowRenderSurface);
                }
                }

                if (fxaa) {
                    { PROFILE("Anti-aliasing(FXAA)");
                    { GPUPROFILE("Anti-aliasing(FXAA)", Video::Query::Type::TIME_ELAPSED);
                    { GPUPROFILE("Anti-aliasing(FXAA)", Video::Query::Type::SAMPLES_PASSED);
                        renderer->AntiAlias(mainWindowRenderSurface);
                    }
                    }
                    }
                }

                { PROFILE("Render particles");
                { GPUPROFILE("Render particles", Video::Query::Type::TIME_ELAPSED);
                    Managers().particleManager->Render(world, position, up, projectionMatrix * viewMatrix, mainWindowRenderSurface);
                }
                }

                if (soundSources || particleEmitters || lightSources || cameras || physics) {
                    { PROFILE("Render editor entities");
                    { GPUPROFILE("Render editor entities", Video::Query::Type::TIME_ELAPSED);
                        RenderEditorEntities(world, soundSources, particleEmitters, lightSources, cameras, physics, position, up, viewMatrix, projectionMatrix, mainWindowRenderSurface);
                    }
                    }
                }

                { PROFILE("Present to back buffer");
                { GPUPROFILE("Present to back buffer", Video::Query::Type::TIME_ELAPSED);
                { GPUPROFILE("Present to back buffer", Video::Query::Type::SAMPLES_PASSED);
                    renderer->Present(mainWindowRenderSurface);
                }
                }
                }
            }
            }

        }

        // Render hmd.
        if (hmdRenderSurface != nullptr) {
            { PROFILE("Render main hmd");
            { GPUPROFILE("Render main hmd", Video::Query::Type::TIME_ELAPSED);

                for (int i = 0; i < 2; ++i) {
                    vr::Hmd_Eye nEye = i == 0 ? vr::Eye_Left : vr::Eye_Right;

                    Lens* lens = camera->GetComponent<Lens>();

                    VRDevice* headset = camera->GetComponent<VRDevice>();

                    const glm::mat4 lensViewMatrix = glm::inverse(camera->GetModelMatrix());
                    const glm::mat4 eyeTranslation = Managers().vrManager->GetHMDHeadToEyeMatrix(nEye);
                    const glm::mat4 eyeViewMatrix = eyeTranslation * lensViewMatrix;
                    const glm::mat4 projectionMatrix = headset->GetHMDProjectionMatrix(nEye, lens->zNear, lens->zFar);
                    const glm::vec3 position = camera->GetWorldPosition();
                    const glm::vec3 up(lensViewMatrix[0][1], lensViewMatrix[1][1], lensViewMatrix[2][1]);

                    { PROFILE("Render world entities");
                    { GPUPROFILE("Render world entities", Video::Query::Type::TIME_ELAPSED);
                        RenderWorldEntities(world, eyeViewMatrix, projectionMatrix, hmdRenderSurface);
                    }
                    }

                    { PROFILE("Render debug entities");
                    { GPUPROFILE("Render debug entities", Video::Query::Type::TIME_ELAPSED);
                        Managers().debugDrawingManager->Render(eyeViewMatrix, projectionMatrix, hmdRenderSurface);
                    }
                    }

                    if (fxaa) {
                        { PROFILE("Anti-aliasing(FXAA)");
                        { GPUPROFILE("Anti-aliasing(FXAA)", Video::Query::Type::TIME_ELAPSED);
                        { GPUPROFILE("Anti-aliasing(FXAA)", Video::Query::Type::SAMPLES_PASSED);
                            renderer->AntiAlias(hmdRenderSurface);
                        }
                        }
                        }
                    }

                    { PROFILE("Render particles");
                    { GPUPROFILE("Render particles", Video::Query::Type::TIME_ELAPSED);
                        Managers().particleManager->Render(world, position, up, projectionMatrix * lensViewMatrix, hmdRenderSurface);
                    }
                    }

                    if (soundSources || particleEmitters || lightSources || cameras || physics) {
                        { PROFILE("Render editor entities");
                        { GPUPROFILE("Render editor entities", Video::Query::Type::TIME_ELAPSED);
                            RenderEditorEntities(world, soundSources, particleEmitters, lightSources, cameras, physics, position, up, lensViewMatrix, projectionMatrix, hmdRenderSurface);
                        }
                        }
                    }

                    hmdRenderSurface->Swap();
                    vr::Texture_t texture = { (void*)(std::uintptr_t)hmdRenderSurface->GetColorTexture()->GetTexture(), vr::TextureType_OpenGL, vr::ColorSpace_Auto };

                    // Submit texture to HMD.
                    Managers().vrManager->Submit(nEye, &texture);
                }
            }
            }

            { PROFILE("Sync hmd");
            { GPUPROFILE("Sync hmd", Video::Query::Type::TIME_ELAPSED);
                Managers().vrManager->Sync();
            }
            }
        }
    }
}

void RenderManager::UpdateBufferSize() {
    delete mainWindowRenderSurface;
    mainWindowRenderSurface = new Video::RenderSurface(MainWindow::GetInstance()->GetSize());
}

void RenderManager::RenderWorldEntities(World& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Video::RenderSurface* renderSurface) {
    // Render from camera.
    renderer->StartRendering(renderSurface);

    // Camera matrices.
    const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    const std::vector<Mesh*>& meshComponents = meshes.GetAll();

    // Render z-pass meshes.
    renderSurface->GetDepthFrameBuffer()->BindWrite();
    { PROFILE("Render z-pass meshes");
    { GPUPROFILE("Render z-pass meshes", Video::Query::Type::TIME_ELAPSED);
    { GPUPROFILE("Render z-pass meshes", Video::Query::Type::SAMPLES_PASSED);
        renderer->PrepareStaticMeshDepthRendering(viewMatrix, projectionMatrix);
        for (Mesh* mesh : meshComponents) {
            if (mesh->IsKilled() || !mesh->entity->enabled)
                continue;

            if (mesh->geometry != nullptr && mesh->geometry->GetType() == Video::Geometry::Geometry3D::STATIC) {
                Entity* entity = mesh->entity;
                // If entity does not have material, it won't be rendered.
                if (entity->GetComponent<Material>() != nullptr)
                    renderer->DepthRenderStaticMesh(mesh->geometry, viewMatrix, projectionMatrix, entity->GetModelMatrix());
            }
        }
    }
    }
    }
    renderSurface->GetDepthFrameBuffer()->Unbind();

    // Render static meshes.
    renderSurface->GetShadingFrameBuffer()->BindWrite();
    { PROFILE("Render static meshes");
    { GPUPROFILE("Render static meshes", Video::Query::Type::TIME_ELAPSED);
    { GPUPROFILE("Render static meshes", Video::Query::Type::SAMPLES_PASSED);

        // Cull lights and update light list.
        LightWorld(world, viewMatrix, projectionMatrix, viewProjectionMatrix);

        // Push matricies and light buffer to the GPU.
        renderer->PrepareStaticMeshRendering(viewMatrix, projectionMatrix);

        // Render meshes.
        for (Mesh* mesh : meshComponents) {
            if (mesh->IsKilled() || !mesh->entity->enabled)
                continue;

            if (mesh->geometry != nullptr && mesh->geometry->GetType() == Video::Geometry::Geometry3D::STATIC) {
                Entity* entity = mesh->entity;
                Material* material = entity->GetComponent<Material>();
                if (material != nullptr)
                    renderer->RenderStaticMesh(mesh->geometry, material->albedo->GetTexture(), material->normal->GetTexture(), material->metallic->GetTexture(), material->roughness->GetTexture(), entity->GetModelMatrix(), false);
            }
        }
    }
    }
    }
    renderSurface->GetShadingFrameBuffer()->Unbind();

    /// @todo Render skinned meshes.
}

void RenderManager::RenderEditorEntities(World& world, bool soundSources, bool particleEmitters, bool lightSources,
    bool cameras, bool physics, const glm::vec3& position, const glm::vec3& up, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
    Video::RenderSurface* renderSurface) {

    const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    renderSurface->GetShadingFrameBuffer()->BindWrite();
    renderer->PrepareRenderingIcons(viewProjectionMatrix, position, up);

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
    renderSurface->GetShadingFrameBuffer()->Unbind();

    // Render physics.
    if (physics) {
        for (Component::Shape* shapeComp : Managers().physicsManager->GetShapeComponents()) {
            const ::Physics::Shape& shape = *shapeComp->GetShape();
            if (shape.GetKind() == ::Physics::Shape::Kind::Sphere) {
                Managers().debugDrawingManager->AddSphere(shapeComp->entity->position, shape.GetSphereData()->radius, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else if (shape.GetKind() == ::Physics::Shape::Kind::Plane) {
                Managers().debugDrawingManager->AddPlane(shapeComp->entity->position, shape.GetPlaneData()->normal, glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }
    }
}

Component::Animation* RenderManager::CreateAnimation() {
    return animations.Create();
}

Component::Animation* RenderManager::CreateAnimation(const Json::Value& node) {
    Component::Animation* animation = animations.Create();
    
    // Load values from Json node.
    std::string name = node.get("riggedModel", "").asString();
    /// @todo Fix animation.
    /*for (Geometry::Model* model : Hymn().models) {
        if (model->name == name)
            riggedModel = model;
    }*/
    
    return animation;
}

const std::vector<Component::Animation*>& RenderManager::GetAnimations() const {
    return animations.GetAll();
}

Component::DirectionalLight* RenderManager::CreateDirectionalLight() {
    return directionalLights.Create();
}

Component::DirectionalLight* RenderManager::CreateDirectionalLight(const Json::Value& node) {
    Component::DirectionalLight* directionalLight = directionalLights.Create();
    
    // Load values from Json node.
    directionalLight->color = Json::LoadVec3(node["color"]);
    directionalLight->ambientCoefficient = node.get("ambientCoefficient", 0.5f).asFloat();
    
    return directionalLight;
}

const std::vector<Component::DirectionalLight*>& RenderManager::GetDirectionalLights() const {
    return directionalLights.GetAll();
}

Component::Lens* RenderManager::CreateLens() {
    return lenses.Create();
}

Component::Lens* RenderManager::CreateLens(const Json::Value& node) {
    Component::Lens* lens = lenses.Create();
    
    // Load values from Json node.
    lens->fieldOfView = node.get("fieldOfView", 45.f).asFloat();
    lens->zNear = node.get("zNear", 0.5f).asFloat();
    lens->zFar = node.get("zFar", 100.f).asFloat();
    
    return lens;
}

const std::vector<Component::Lens*>& RenderManager::GetLenses() const {
    return lenses.GetAll();
}

Component::Material* RenderManager::CreateMaterial() {
    return materials.Create();
}

Component::Material* RenderManager::CreateMaterial(const Json::Value& node) {
    Component::Material* material = materials.Create();
    
    // Load values from Json node.
    LoadTexture(material->albedo, node.get("albedo", "").asString());
    LoadTexture(material->normal, node.get("normal", "").asString());
    LoadTexture(material->metallic, node.get("metallic", "").asString());
    LoadTexture(material->roughness, node.get("roughness", "").asString());
    
    return material;
}

const std::vector<Component::Material*>& RenderManager::GetMaterials() const {
    return materials.GetAll();
}

Component::Mesh* RenderManager::CreateMesh() {
    return meshes.Create();
}

Component::Mesh* RenderManager::CreateMesh(const Json::Value& node) {
    Component::Mesh* mesh = meshes.Create();
    
    // Load values from Json node.
    std::string meshName = node.get("model", "").asString();
    mesh->geometry = Managers().resourceManager->CreateModel(meshName);
    
    return mesh;
}

const std::vector<Component::Mesh*>& RenderManager::GetMeshes() const {
    return meshes.GetAll();
}

Component::PointLight* RenderManager::CreatePointLight() {
    return pointLights.Create();
}

Component::PointLight* RenderManager::CreatePointLight(const Json::Value& node) {
    Component::PointLight* pointLight = pointLights.Create();
    
    // Load values from Json node.
    pointLight->color = Json::LoadVec3(node["color"]);
    pointLight->attenuation = node.get("attenuation", 1.f).asFloat();
    pointLight->intensity = node.get("intensity", 1.f).asFloat();
    
    return pointLight;
}

const std::vector<Component::PointLight*>& RenderManager::GetPointLights() const {
    return pointLights.GetAll();
}

Component::SpotLight* RenderManager::CreateSpotLight() {
    return spotLights.Create();
}

Component::SpotLight* RenderManager::CreateSpotLight(const Json::Value& node) {
    Component::SpotLight* spotLight = spotLights.Create();
    
    // Load values from Json node.
    spotLight->color = Json::LoadVec3(node["color"]);
    spotLight->ambientCoefficient = node.get("ambientCoefficient", 0.5f).asFloat();
    spotLight->attenuation = node.get("attenuation", 1.f).asFloat();
    spotLight->intensity = node.get("intensity", 1.f).asFloat();
    spotLight->coneAngle = node.get("coneAngle", 15.f).asFloat();
    
    return spotLight;
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

void RenderManager::SetGamma(float gamma) {
    Hymn().filterSettings.gamma = gamma;
}

float RenderManager::GetGamma() const {
    return Hymn().filterSettings.gamma;
}

void RenderManager::SetFogApply(bool fogApply) {
    Hymn().filterSettings.fogApply = fogApply;
}

bool RenderManager::GetFogApply() const {
    return Hymn().filterSettings.fogApply;
}

void RenderManager::SetFogDensity(float fogDensity) {
    Hymn().filterSettings.fogDensity = fogDensity;
}

float RenderManager::GetFogDensity() const {
    return Hymn().filterSettings.fogDensity;
}

void RenderManager::SetFogColor(const glm::vec3& fogColor) {
    Hymn().filterSettings.fogColor = fogColor;
}

glm::vec3 RenderManager::GetFogColor() const {
    return Hymn().filterSettings.fogColor;
}

void RenderManager::SetColorFilterApply(bool colorFilterApply) {
    Hymn().filterSettings.colorFilterApply = colorFilterApply;
}

bool RenderManager::GetColorFilterApply() const {
    return Hymn().filterSettings.colorFilterApply;
}

void RenderManager::SetColorFilterColor(const glm::vec3& colorFilterColor) {
    Hymn().filterSettings.colorFilterColor = colorFilterColor;
}

glm::vec3 RenderManager::GetColorFilterColor() const {
    return Hymn().filterSettings.colorFilterColor;
}

void RenderManager::SetDitherApply(bool ditherApply) {
    Hymn().filterSettings.ditherApply = ditherApply;
}

bool RenderManager::GetDitherApply() const {
    return Hymn().filterSettings.ditherApply;
}

void RenderManager::LightWorld(World& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& viewProjectionMatrix) {

    std::vector<Video::Light> lights;

    float cutOff;
    Video::AxisAlignedBoundingBox aabb(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Add all directional lights.
    for (Component::DirectionalLight* directionalLight : directionalLights.GetAll()) {
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
        lights.push_back(light);
    }
    
    // Add all spot lights.
    for (Component::SpotLight* spotLight : spotLights.GetAll()) {
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
        lights.push_back(light);
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
        
        Video::Frustum frustum(viewProjectionMatrix * modelMat);
        if (frustum.Collide(aabb)) {
            glm::mat4 modelMatrix(lightEntity->GetModelMatrix());
            Video::Light light;
            light.position = viewMatrix * (glm::vec4(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), 1.0));
            light.intensities = pointLight->color * pointLight->intensity;
            light.attenuation = pointLight->attenuation;
            light.ambientCoefficient = 0.f;
            light.coneAngle = 180.f;
            light.direction = glm::vec3(1.f, 0.f, 0.f);
            lights.push_back(light);
        }
    }
    
    // Update light buffer.
    renderer->SetLights(lights);
}

void RenderManager::LoadTexture(TextureAsset*& texture, const std::string& name) {
    if (!name.empty())
        texture = Managers().resourceManager->CreateTextureAsset(name);
}
