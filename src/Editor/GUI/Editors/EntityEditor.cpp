#include "EntityEditor.hpp"

#include <Engine/Component/Animation.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/DirectionalLight.hpp>
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/SpotLight.hpp>
#include <Engine/Component/Listener.hpp>
#include <Engine/Component/Script.hpp>
#include <Engine/Component/SoundSource.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Geometry/Model.hpp>
#include <Engine/Texture/TextureAsset.hpp>
#include <Video/Texture/Texture2D.hpp>
#include <Engine/Audio/SoundBuffer.hpp>
#include <Engine/Script/ScriptFile.hpp>
#include <Engine/Util/FileSystem.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ScriptManager.hpp>
#include <Engine/Manager/ParticleManager.hpp>
#include <Engine/Hymn.hpp>

#include "../../Util/EditorSettings.hpp"
#include "../FileSelector.hpp"
#include "../../ImGui/GuiHelpers.hpp"
#include "../../Resources.hpp"

using namespace GUI;

EntityEditor::EntityEditor() {
    name[0] = '\0';
    AddEditor<Component::Animation>("Animation", std::bind(&EntityEditor::AnimationEditor, this, std::placeholders::_1));
    AddEditor<Component::Physics>("Physics", std::bind(&EntityEditor::PhysicsEditor, this, std::placeholders::_1));
    AddEditor<Component::Mesh>("Mesh", std::bind(&EntityEditor::MeshEditor, this, std::placeholders::_1));
    AddEditor<Component::Lens>("Lens", std::bind(&EntityEditor::LensEditor, this, std::placeholders::_1));
    AddEditor<Component::Material>("Material", std::bind(&EntityEditor::MaterialEditor, this, std::placeholders::_1));
    AddEditor<Component::DirectionalLight>("Directional light", std::bind(&EntityEditor::DirectionalLightEditor, this, std::placeholders::_1));
    AddEditor<Component::PointLight>("Point light", std::bind(&EntityEditor::PointLightEditor, this, std::placeholders::_1));
    AddEditor<Component::SpotLight>("Spot light", std::bind(&EntityEditor::SpotLightEditor, this, std::placeholders::_1));
    AddEditor<Component::Listener>("Listener", std::bind(&EntityEditor::ListenerEditor, this, std::placeholders::_1));
    AddEditor<Component::Script>("Script", std::bind(&EntityEditor::ScriptEditor, this, std::placeholders::_1));
    AddEditor<Component::SoundSource>("Sound source", std::bind(&EntityEditor::SoundSourceEditor, this, std::placeholders::_1));
    AddEditor<Component::ParticleEmitter>("Particle emitter", std::bind(&EntityEditor::ParticleEmitterEditor, this, std::placeholders::_1));
}

EntityEditor::~EntityEditor() {
    
}

void EntityEditor::Show() {
    if (ImGui::Begin(("Entity: " + entity->name + "###" + std::to_string(reinterpret_cast<uintptr_t>(entity))).c_str(), &visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders)) {
        ImGui::InputText("Name", name, 128);
        entity->name = name;
        ImGui::Text("Transform");
        ImGui::ShowHelpMarker("The entity's position, rotation and scale.", 75.f);
        ImGui::Indent();
        ImGui::DraggableVec3("Position", entity->position);
        ImGui::DraggableVec3("Rotation", entity->rotation);
        ImGui::DraggableVec3("Scale", entity->scale);
        ImGui::Unindent();
        if (!entity->IsScene()) {
            if (ImGui::Button("Add component"))
                ImGui::OpenPopup("Add component");
            
            if (ImGui::BeginPopup("Add component")) {
                ImGui::Text("Components");
                ImGui::Separator();
                
                for (Editor& editor : editors) {
                    editor.addFunction();
                }
                
                ImGui::EndPopup();
            }
            
            for (Editor& editor : editors) {
                editor.editFunction();
            }
        }
    }

    ImGui::End();
}

void EntityEditor::SetEntity(Entity* entity) {
    this->entity = entity;
    strcpy(name, entity->name.c_str());
}

Entity* EntityEditor::GetEntity() {
    return entity;
}

bool EntityEditor::ShowsEntity(Entity* entity) {
    return this->entity == entity;
}

bool EntityEditor::IsVisible() const {
    return visible;
}

void EntityEditor::SetVisible(bool visible) {
    this->visible = visible;
}

void EntityEditor::AnimationEditor(Component::Animation* animation) {
    ImGui::Indent();
    if (ImGui::Button("Select model##Animation"))
        ImGui::OpenPopup("Select model##Animation");

    if (ImGui::BeginPopup("Select model##Animation")) {
        ImGui::Text("Models");
        ImGui::Separator();

        for (Geometry::Model* model : Resources().models) {
            if (ImGui::Selectable(model->name.c_str()))
                animation->riggedModel = dynamic_cast<Geometry::Model*>(model);
        }

        ImGui::EndPopup();
    }
    ImGui::Unindent();
}

void EntityEditor::PhysicsEditor(Component::Physics* physics) {
    ImGui::Text("Positional");
    ImGui::Indent();
    ImGui::DraggableVec3("Velocity", physics->velocity);
    ImGui::DraggableFloat("Max velocity", physics->maxVelocity, 0.0f);
    ImGui::DraggableVec3("Acceleration", physics->acceleration);
    ImGui::DraggableFloat("Velocity drag factor", physics->velocityDragFactor);
    ImGui::DraggableFloat("Gravity factor", physics->gravityFactor);
    ImGui::Unindent();
    ImGui::Text("Angular");
    ImGui::Indent();
    ImGui::DraggableVec3("Angular velocity", physics->angularVelocity);
    ImGui::DraggableFloat("Max angular velocity", physics->maxAngularVelocity, 0.0f);
    ImGui::DraggableVec3("Angular acceleration", physics->angularAcceleration);
    ImGui::DraggableFloat("Angular drag factor", physics->angularDragFactor);
    ImGui::DraggableVec3("Moment of inertia", physics->momentOfInertia);
    ImGui::Unindent();

}

void EntityEditor::MeshEditor(Component::Mesh* mesh) {
    ImGui::Indent();
    if (ImGui::Button("Select model##Mesh"))
        ImGui::OpenPopup("Select model##Mesh");
    
    if (ImGui::BeginPopup("Select model##Mesh")) {
        ImGui::Text("Models");
        ImGui::Separator();
        
        for (Geometry::Model* model : Resources().models) {
            if (ImGui::Selectable(model->name.c_str()))
                mesh->geometry = model;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();
}

void EntityEditor::LensEditor(Component::Lens* lens) {
    ImGui::Indent();
    ImGui::DraggableFloat("Field of view", lens->fieldOfView, 0.0f, 180.f);
    ImGui::DraggableFloat("Z near", lens->zNear, 0.0f);
    ImGui::DraggableFloat("Z far", lens->zFar, 0.0f);
    ImGui::Unindent();
}

void EntityEditor::MaterialEditor(Component::Material* material) {
    // Albedo
    ImGui::Text("Albedo");
    ImGui::Indent();
    if (material->albedo->GetTexture()->IsLoaded())
        ImGui::Image((void*) material->albedo->GetTexture()->GetTextureID(), ImVec2(128, 128));
    
    if (ImGui::Button("Select albedo texture"))
        ImGui::OpenPopup("Select albedo texture");
    
    if (ImGui::BeginPopup("Select albedo texture")) {
        ImGui::Text("Textures");
        ImGui::Separator();
        
        for (TextureAsset* texture : Resources().textures) {
            if (ImGui::Selectable(texture->name.c_str()))
                material->albedo = texture;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();


    // Normal
    ImGui::Text("Normal");
    ImGui::Indent();
    if (material->normal->GetTexture()->IsLoaded())
        ImGui::Image((void*) material->normal->GetTexture()->GetTextureID(), ImVec2(128, 128));
    
    if (ImGui::Button("Select normal texture"))
        ImGui::OpenPopup("Select normal texture");
    
    if (ImGui::BeginPopup("Select normal texture")) {
        ImGui::Text("Textures");
        ImGui::Separator();
        
        for (TextureAsset* texture : Resources().textures) {
            if (ImGui::Selectable(texture->name.c_str()))
                material->normal = texture;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();

    // Metallic
    ImGui::Text("Metallic");
    ImGui::Indent();
    if (material->metallic->GetTexture()->IsLoaded())
        ImGui::Image((void*) material->metallic->GetTexture()->GetTextureID(), ImVec2(128, 128));
    
    if (ImGui::Button("Select metallic texture"))
        ImGui::OpenPopup("Select metallic texture");
    
    if (ImGui::BeginPopup("Select metallic texture")) {
        ImGui::Text("Textures");
        ImGui::Separator();
        
        for (TextureAsset* texture : Resources().textures) {
            if (ImGui::Selectable(texture->name.c_str()))
                material->metallic = texture;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();

    // Roughness
    ImGui::Text("Roughness");
    ImGui::Indent();
    if (material->roughness->GetTexture()->IsLoaded())
        ImGui::Image((void*) material->roughness->GetTexture()->GetTextureID(), ImVec2(128, 128));
    
    if (ImGui::Button("Select roughness texture"))
        ImGui::OpenPopup("Select roughness texture");
    
    if (ImGui::BeginPopup("Select roughness texture")) {
        ImGui::Text("Textures");
        ImGui::Separator();
        
        for (TextureAsset* texture : Resources().textures) {
            if (ImGui::Selectable(texture->name.c_str()))
                material->roughness = texture;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();
}

void EntityEditor::DirectionalLightEditor(Component::DirectionalLight* directionalLight) {
    ImGui::Indent();
    ImGui::ColorEdit3("Color", &directionalLight->color[0]);
    ImGui::DraggableFloat("Ambient coefficient", directionalLight->ambientCoefficient, 0.0f);
    ImGui::Unindent();
}

void EntityEditor::PointLightEditor(Component::PointLight* pointLight) {
    ImGui::Indent();
    ImGui::ColorEdit3("Color", &pointLight->color[0]);
    ImGui::DraggableFloat("Ambient coefficient", pointLight->ambientCoefficient, 0.0f);
    ImGui::DraggableFloat("Attenuation", pointLight->attenuation, 0.0f);
    ImGui::DraggableFloat("Intensity", pointLight->intensity, 0.0f);
    ImGui::Unindent();
}

void EntityEditor::SpotLightEditor(Component::SpotLight* spotLight) {
    ImGui::Indent();
    ImGui::ColorEdit3("Color", &spotLight->color[0]);
    ImGui::DraggableFloat("Ambient coefficient", spotLight->ambientCoefficient, 0.0f);
    ImGui::DraggableFloat("Attenuation", spotLight->attenuation, 0.0f);
    ImGui::DraggableFloat("Intensity", spotLight->intensity, 0.0f);
    ImGui::DraggableFloat("Cone angle", spotLight->coneAngle, 0.0f, 180.f);
    ImGui::Unindent();
}

void EntityEditor::ListenerEditor(Component::Listener* listener) {
    
}

void EntityEditor::ScriptEditor(Component::Script* script) {
    ImGui::Indent();
    if(script->scriptFile != nullptr)
        ImGui::Text(script->scriptFile->name.c_str());
    else
        ImGui::Text("No script loaded");
    
    if (ImGui::Button("Select script"))
        ImGui::OpenPopup("Select script");

    if (ImGui::BeginPopup("Select script")) {
        ImGui::Text("Scripts");
        ImGui::Separator();

        for (ScriptFile* scriptFile : Hymn().scripts) {
            if (ImGui::Selectable(scriptFile->name.c_str()))
                script->scriptFile = scriptFile;
        }

        ImGui::EndPopup();
    }
    ImGui::Unindent();
}

void EntityEditor::SoundSourceEditor(Component::SoundSource* soundSource) {
    ImGui::Text("Sound");
    ImGui::Indent();
    if (ImGui::Button("Select sound"))
        ImGui::OpenPopup("Select sound");
    
    if (ImGui::BeginPopup("Select sound")) {
        ImGui::Text("Sounds");
        ImGui::Separator();
        
        for (Audio::SoundBuffer* sound : Resources().sounds) {
            if (ImGui::Selectable(sound->name.c_str()))
                soundSource->soundBuffer = sound;
        }
        
        ImGui::EndPopup();
    }
    ImGui::Unindent();
    ImGui::Text("Sound properties");
    ImGui::Indent();
    ImGui::DraggableFloat("Pitch", soundSource->pitch, 0.0f);
    ImGui::DraggableFloat("Gain", soundSource->gain, 0.0f);
    ImGui::Checkbox("Loop", &soundSource->loop);
    ImGui::Unindent();
}

void EntityEditor::ParticleEmitterEditor(Component::ParticleEmitter* particleEmitter) {
    ImGui::Text("Particle");
    ImGui::Indent();
    int rows = Managers().particleManager->GetTextureAtlasRows();
    float column = static_cast<float>(particleEmitter->particleType.textureIndex % rows);
    float row = static_cast<float>(particleEmitter->particleType.textureIndex / rows);
    ImGui::Image((void*) Managers().particleManager->GetTextureAtlas()->GetTextureID(), ImVec2(128, 128), ImVec2(column / rows, row / rows), ImVec2((column + 1.f) / rows, (row + 1.f) / rows));
    ImGui::InputInt("Texture index", &particleEmitter->particleType.textureIndex);
    ImGui::ColorEdit3("Color", &particleEmitter->particleType.color[0]);
    ImGui::DraggableVec3("Min velocity", particleEmitter->particleType.minVelocity);
    ImGui::DraggableVec3("Max velocity", particleEmitter->particleType.maxVelocity);
    ImGui::DraggableFloat("Average lifetime", particleEmitter->particleType.averageLifetime, 0.0f);
    ImGui::DraggableFloat("Lifetime variance", particleEmitter->particleType.lifetimeVariance, 0.0f);
    ImGui::DraggableVec2("Average size", particleEmitter->particleType.averageSize, 0.0f);
    ImGui::DraggableVec2("Size variance", particleEmitter->particleType.sizeVariance, 0.0f);
    ImGui::Checkbox("Uniform scaling", &particleEmitter->particleType.uniformScaling);
    ImGui::DraggableFloat("Start alpha", particleEmitter->particleType.startAlpha, 0.0f, 1.0f);
    ImGui::DraggableFloat("Mid alpha", particleEmitter->particleType.midAlpha, 0.0f, 1.0f);
    ImGui::DraggableFloat("End alpha", particleEmitter->particleType.endAlpha, 0.0f, 1.0f);
    ImGui::Unindent();
    
    ImGui::Text("Emitter");
    ImGui::Indent();
    ImGui::DraggableFloat("Average emit time", particleEmitter->averageEmitTime, 0.0f);
    ImGui::DraggableFloat("Emit time variance", particleEmitter->emitTimeVariance, 0.0f);
    
    const char* items[] = { "Point", "Cuboid" };
    int item = static_cast<int>(particleEmitter->emitterType);
    if (ImGui::Combo("Emitter type", &item, items, 2))
        particleEmitter->emitterType = static_cast<Component::ParticleEmitter::EmitterType>(item);
    
    if (particleEmitter->emitterType == Component::ParticleEmitter::CUBOID)
        ImGui::DraggableVec3("Size", particleEmitter->size);
    
    ImGui::Unindent();
    
    ImGui::Text("Preview");
    ImGui::Indent();
    ImGui::Checkbox("Simulate", &particleEmitter->preview);
    ImGui::Unindent();
}
