#include "RenderManager.hpp"

#include <GL/glew.h>
#include <Video/Renderer.hpp>
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include "ParticleManager.hpp"
#include "DebugDrawingManager.hpp"
#include "EditorEntity.vert.hpp"
#include "EditorEntity.geom.hpp"
#include "EditorEntity.frag.hpp"
#include "Light.png.hpp"
#include "ParticleEmitter.png.hpp"
#include "SoundSource.png.hpp"
#include "Camera.png.hpp"
#include <Video/Shader/ShaderProgram.hpp>
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
#include "../Geometry/RiggedModel.hpp"
#include "../Geometry/Skeleton.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Lighting/DeferredLighting.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Video/Culling/Frustum.hpp>
#include "../MainWindow.hpp"
#include "../RenderTarget.hpp"
#include "../PostProcessing/PostProcessing.hpp"
#include <Video/PostProcessing/ColorFilter.hpp>
#include <Video/PostProcessing/FogFilter.hpp>
#include <Video/PostProcessing/FXAAFilter.hpp>
#include <Video/PostProcessing/GammaCorrectionFilter.hpp>
#include <Video/PostProcessing/GlowFilter.hpp>
#include <Video/PostProcessing/GlowBlurFilter.hpp>
#include "../Hymn.hpp"

using namespace Component;

RenderManager::RenderManager() {
    renderer = new Video::Renderer(MainWindow::GetInstance()->GetSize());
    
    // Init shaders.
    editorEntityVertexShader = Managers().resourceManager->CreateShader(EDITORENTITY_VERT, EDITORENTITY_VERT_LENGTH, GL_VERTEX_SHADER);
    editorEntityGeometryShader = Managers().resourceManager->CreateShader(EDITORENTITY_GEOM, EDITORENTITY_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    editorEntityFragmentShader = Managers().resourceManager->CreateShader(EDITORENTITY_FRAG, EDITORENTITY_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    editorEntityShaderProgram = Managers().resourceManager->CreateShaderProgram({ editorEntityVertexShader, editorEntityGeometryShader, editorEntityFragmentShader });
    
    // Init textures.
    particleEmitterTexture = Managers().resourceManager->CreateTexture2D(PARTICLEEMITTER_PNG, PARTICLEEMITTER_PNG_LENGTH);
    lightTexture = Managers().resourceManager->CreateTexture2D(LIGHT_PNG, LIGHT_PNG_LENGTH);
    soundSourceTexture = Managers().resourceManager->CreateTexture2D(SOUNDSOURCE_PNG, SOUNDSOURCE_PNG_LENGTH);
    cameraTexture = Managers().resourceManager->CreateTexture2D(CAMERA_PNG, CAMERA_PNG_LENGTH);
    
    deferredLighting = new DeferredLighting(renderer);
    
    // Init filters.
    postProcessing = new PostProcessing();
    colorFilter = new Video::ColorFilter(glm::vec3(1.f, 1.f, 1.f));
    fogFilter = new Video::FogFilter(glm::vec3(1.f, 1.f, 1.f));
    fxaaFilter = new Video::FXAAFilter();
    gammaCorrectionFilter = new Video::GammaCorrectionFilter();
    glowFilter = new Video::GlowFilter();
    glowBlurFilter = new Video::GlowBlurFilter();
    
    // Create editor entity geometry.
    float vertex;
    
    glBindVertexArray(0);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(float), &vertex, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
    
    glBindVertexArray(0);
}

RenderManager::~RenderManager() {
    Managers().resourceManager->FreeShader(editorEntityVertexShader);
    Managers().resourceManager->FreeShader(editorEntityGeometryShader);
    Managers().resourceManager->FreeShader(editorEntityFragmentShader);
    Managers().resourceManager->FreeShaderProgram(editorEntityShaderProgram);
    
    Managers().resourceManager->FreeTexture2D(particleEmitterTexture);
    Managers().resourceManager->FreeTexture2D(lightTexture);
    Managers().resourceManager->FreeTexture2D(soundSourceTexture);
    Managers().resourceManager->FreeTexture2D(cameraTexture);
    
    delete deferredLighting;
    
    delete postProcessing;
    delete colorFilter;
    delete fogFilter;
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete glowFilter;
    delete glowBlurFilter;
    
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);

    delete renderer;
}

void RenderManager::Render(World& world, Entity* camera) {
    renderer->Clear();
    
    // Find camera entity.
    if (camera == nullptr) {
        std::vector<Lens*> lenses = world.GetComponents<Lens>();
        for (Lens* lens : lenses) {
            camera = lens->entity;
        }
    }
    
    // Render from camera.
    if (camera != nullptr) {
        glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
        renderer->StartRendering();
        
        // Camera matrices.
        glm::mat4 viewMatrix = camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->position);
        glm::mat4 projectionMatrix = camera->GetComponent<Lens>()->GetProjection(screenSize);
        
        std::vector<Mesh*> meshes = world.GetComponents<Mesh>();
        
        // Render static meshes.
        renderer->PrepareStaticMeshRendering(viewMatrix, projectionMatrix);
        for (Mesh* mesh : meshes) {
            if (mesh->geometry != nullptr && mesh->geometry->GetType() == Video::Geometry::Geometry3D::STATIC) {
                Entity* entity = mesh->entity;
                Material* material = entity->GetComponent<Material>();
                if (material != nullptr) {
                    glm::mat4 modelMatrix = entity->GetModelMatrix();
                    renderer->RenderStaticMesh(mesh->geometry, material->diffuse, material->normal, material->specular, material->glow, modelMatrix);
                }
            }
        }
        
        // Render skinned meshes.
        renderer->PrepareSkinnedMeshRendering(viewMatrix, projectionMatrix);
        for (Mesh* mesh : meshes) {
            if (mesh->geometry != nullptr && mesh->geometry->GetType() == Video::Geometry::Geometry3D::SKIN) {
                Entity* entity = mesh->entity;
                Material* material = entity->GetComponent<Material>();
                if (material != nullptr) {
                    glm::mat4 modelMatrix = entity->GetModelMatrix();
                    Geometry::RiggedModel* model = static_cast<Geometry::RiggedModel*>(mesh->geometry);
                    renderer->RenderSkinnedMesh(mesh->geometry, material->diffuse, material->normal, material->specular, material->glow, modelMatrix, model->skeleton.GetFinalTransformations(), model->skeleton.GetFinalTransformationsIT());
                }
            }
        }
        
        // Light the world.
        postProcessing->GetRenderTarget()->SetTarget();
        deferredLighting->Render(world, camera);
        
        // Anti-aliasing.
        if (Hymn().filterSettings.fxaa) {
            fxaaFilter->SetScreenSize(screenSize);
            postProcessing->ApplyFilter(fxaaFilter);
        }
        
        // Fog.
        if (Hymn().filterSettings.fog) {
            fogFilter->SetProjectionMatrix(camera->GetComponent<Component::Lens>()->GetProjection(screenSize));
            fogFilter->SetDensity(Hymn().filterSettings.fogDensity);
            fogFilter->SetColor(Hymn().filterSettings.fogColor);
            postProcessing->ApplyFilter(fogFilter);
        }
        
        // Render particles.
        Managers().particleManager->UpdateBuffer(world);
        Managers().particleManager->Render(world, camera);
        
        // Glow.
        if (Hymn().filterSettings.glow) {
            glowBlurFilter->SetScreenSize(screenSize);
            for (int i = 0; i < Hymn().filterSettings.glowBlurAmount; ++i) {
                glowBlurFilter->SetHorizontal(true);
                postProcessing->ApplyFilter(glowBlurFilter);
                glowBlurFilter->SetHorizontal(false);
                postProcessing->ApplyFilter(glowBlurFilter);
            }
            postProcessing->ApplyFilter(glowFilter);
        }
        
        // Color.
        if (Hymn().filterSettings.color) {
            colorFilter->SetColor(Hymn().filterSettings.colorColor);
            postProcessing->ApplyFilter(colorFilter);
        }
        
        // Gamma correction.
        postProcessing->ApplyFilter(gammaCorrectionFilter);
        
        // Render to back buffer.
        postProcessing->Render(true);
    }
}

void RenderManager::RenderEditorEntities(World& world, Entity* camera, bool soundSources, bool particleEmitters, bool lightSources, bool cameras) {
    // Find camera entity.
    if (camera == nullptr) {
        std::vector<Lens*> lenses = world.GetComponents<Lens>();
        for (Lens* lens : lenses) {
            camera = lens->entity;
        }
    }
    
    // Render from camera.
    if (camera != nullptr) {
        editorEntityShaderProgram->Use();
        glBindVertexArray(vertexArray);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        // Set camera uniforms.
        glm::vec2 screenSize(MainWindow::GetInstance()->GetSize());
        glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->position));
        glm::mat4 projectionMat(camera->GetComponent<Lens>()->GetProjection(screenSize));
        glm::mat4 viewProjectionMat(projectionMat * viewMat);
        glm::vec3 up(glm::inverse(camera->GetCameraOrientation())* glm::vec4(0, 1, 0, 1));
    
        glUniformMatrix4fv(editorEntityShaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &viewProjectionMat[0][0]);
        glUniform3fv(editorEntityShaderProgram->GetUniformLocation("cameraPosition"), 1, &camera->position[0]);
        glUniform3fv(editorEntityShaderProgram->GetUniformLocation("cameraUp"), 1, &up[0]);
        glUniform1i(editorEntityShaderProgram->GetUniformLocation("baseImage"), 0);
        
        glActiveTexture(GL_TEXTURE0);
        
        // Render sound sources.
        if (soundSources) {
            glBindTexture(GL_TEXTURE_2D, soundSourceTexture->GetTextureID());
            
            for (SoundSource* soundSource : world.GetComponents<SoundSource>())
                RenderEditorEntity(soundSource);
        }
        
        // Render particle emitters.
        if (particleEmitters) {
            glBindTexture(GL_TEXTURE_2D, particleEmitterTexture->GetTextureID());
            
            for (ParticleEmitter* emitter : world.GetComponents<ParticleEmitter>())
                RenderEditorEntity(emitter);
        }
        
        // Render light sources.
        if (lightSources) {
            glBindTexture(GL_TEXTURE_2D, lightTexture->GetTextureID());
            
            for (DirectionalLight* light : world.GetComponents<DirectionalLight>())
                RenderEditorEntity(light);
            
            for (PointLight* light : world.GetComponents<PointLight>())
                RenderEditorEntity(light);
            
            for (SpotLight* light : world.GetComponents<SpotLight>())
                RenderEditorEntity(light);
        }
        
        // Render cameras.
        if (cameras) {
            glBindTexture(GL_TEXTURE_2D, cameraTexture->GetTextureID());
            
            for (Lens* lens : world.GetComponents<Lens>())
                RenderEditorEntity(lens);
        }
        
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}

void RenderManager::UpdateBufferSize() {
    postProcessing->UpdateBufferSize();
    
    delete deferredLighting;
    deferredLighting = new DeferredLighting(renderer);
}

void RenderManager::RenderEditorEntity(SuperComponent* component) {
    Entity* entity = component->entity;
    glUniform3fv(editorEntityShaderProgram->GetUniformLocation("position"), 1, &entity->GetWorldPosition()[0]);
    glDrawArrays(GL_POINTS, 0, 1);
}
