#include "ResourceList.hpp"

#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>
#include <Engine/Geometry/Rectangle.hpp>
#include <Engine/Geometry/OBJModel.hpp>
#include <Engine/Font/Font.hpp>
#include <Engine/Texture/Texture2D.hpp>
#include <Engine/Audio/SoundBuffer.hpp>
#include "ABeeZee.ttf.hpp"
#include "Add.png.hpp"

#include <Engine/Hymn.hpp>
#include <Engine/Scene/Scene.hpp>
#include <Engine/Util/Input.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Physics/Rectangle.hpp>

using namespace GUI;

ResourceList::ResourceList(Widget* parent) : Widget(parent) {
    rectangle = Managers().resourceManager->CreateRectangle();
    font = Managers().resourceManager->CreateFontEmbedded(ABEEZEE_TTF, ABEEZEE_TTF_LENGTH, 16.f);
    addTexture = Managers().resourceManager->CreateTexture2D(ADD_PNG, ADD_PNG_LENGTH);
}

ResourceList::~ResourceList() {
    Managers().resourceManager->FreeRectangle();
    Managers().resourceManager->FreeFont(font);
    Managers().resourceManager->FreeTexture2D(addTexture);
}

void ResourceList::Update() {
    glm::vec2 mousePosition(Input()->CursorX(), Input()->CursorY());
    glm::vec2 position(GetPosition());
    Physics::Rectangle rect(position + glm::vec2(font->GetWidth("Entities") + 5.f, 6.f), glm::vec2(10.f, 10.f));
    addEntityHover = rect.Collide(mousePosition);
    
    position.y += (Hymn().activeScene.GetEntities().size() + 1) * font->GetHeight();
    rect = Physics::Rectangle(position + glm::vec2(font->GetWidth("Models") + 5.f, 6.f), glm::vec2(10.f, 10.f));
    addModelHover = rect.Collide(mousePosition);
    
    position.y += (Hymn().models.size() + 1) * font->GetHeight();
    rect = Physics::Rectangle(position + glm::vec2(font->GetWidth("Textures") + 5.f, 6.f), glm::vec2(10.f, 10.f));
    addTextureHover = rect.Collide(mousePosition);
    
    position.y += (Hymn().textures.size() + 1) * font->GetHeight();
    rect = Physics::Rectangle(position + glm::vec2(font->GetWidth("Sounds") + 5.f, 6.f), glm::vec2(10.f, 10.f));
    addSoundHover = rect.Collide(mousePosition);
    
    if (Input()->Triggered(InputHandler::CLICK)) {
        if (addEntityHover) {
            // Add entity button pressed.
            Hymn().activeScene.CreateEntity("Entity #" + std::to_string(Hymn().entityNumber++));
        } else if (addModelHover) {
            // Add model button pressed.
            Geometry::OBJModel* model = new Geometry::OBJModel();
            model->name = "Model #" + std::to_string(Hymn().modelNumber++);
            Hymn().models.push_back(model);
        } else if (addTextureHover) {
            // Add texture button pressed.
            Texture2D* texture = new Texture2D();
            texture->name = "Texture #" + std::to_string(Hymn().textureNumber++);
            Hymn().textures.push_back(texture);
        } else if (addSoundHover) {
            // Add sound button pressed.
            Audio::SoundBuffer* sound = new Audio::SoundBuffer();
            sound->name = "Sound #" + std::to_string(Hymn().soundNumber++);
            Hymn().sounds.push_back(sound);
        } else {
            position  = GetPosition();
            
            // Check if entity selected.
            for (Entity* entity : Hymn().activeScene.GetEntities()) {
                position.y += font->GetHeight();
                rect = Physics::Rectangle(position, glm::vec2(size.x, font->GetHeight()));
                if (rect.Collide(mousePosition)) {
                    selectedEntity = entity;
                    selectedModel = nullptr;
                    selectedTexture = nullptr;
                    selectedSound = nullptr;
                    if (hasEntitySelectedCallback)
                        entitySelectedCallback(entity);
                    break;
                }
            }
            
            position  = GetPosition();
            position.y += (1 + Hymn().activeScene.GetEntities().size()) * font->GetHeight();
            
            // Check if model selected.
            for (Geometry::OBJModel* model : Hymn().models) {
                position.y += font->GetHeight();
                rect = Physics::Rectangle(position, glm::vec2(size.x, font->GetHeight()));
                if (rect.Collide(mousePosition)) {
                    selectedEntity = nullptr;
                    selectedModel = model;
                    selectedTexture = nullptr;
                    selectedSound = nullptr;
                    if (hasModelSelectedCallback)
                        modelSelectedCallback(model);
                    break;
                }
            }
            
            position  = GetPosition();
            position.y += (2 + Hymn().activeScene.GetEntities().size() + Hymn().models.size()) * font->GetHeight();
            
            // Check if texture selected.
            for (Texture2D* texture : Hymn().textures) {
                position.y += font->GetHeight();
                rect = Physics::Rectangle(position, glm::vec2(size.x, font->GetHeight()));
                if (rect.Collide(mousePosition)) {
                    selectedEntity = nullptr;
                    selectedModel = nullptr;
                    selectedTexture = texture;
                    selectedSound = nullptr;
                    if (hasTextureSelectedCallback)
                        textureSelectedCallback(texture);
                    break;
                }
            }
            
            position  = GetPosition();
            position.y += (3 + Hymn().activeScene.GetEntities().size() + Hymn().models.size() + Hymn().textures.size()) * font->GetHeight();
            
            // Check if sound selected.
            for (Audio::SoundBuffer* sound : Hymn().sounds) {
                position.y += font->GetHeight();
                rect = Physics::Rectangle(position, glm::vec2(size.x, font->GetHeight()));
                if (rect.Collide(mousePosition)) {
                    selectedEntity = nullptr;
                    selectedModel = nullptr;
                    selectedTexture = nullptr;
                    selectedSound = sound;
                    if (hasSoundSelectedCallback)
                        soundSelectedCallback(sound);
                    break;
                }
            }
        }
    }
}

void ResourceList::Render() {
    glm::vec3 color(0.06666666666f, 0.06274509803f, 0.08235294117f);
    glm::vec2 position = GetPosition();
    rectangle->Render(position, size, color);
    
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
    font->RenderText("Entities", position, GetSize().x);
    addTexture->Render(position + glm::vec2(font->GetWidth("Entities") + 5.f, 6.f), glm::vec2(addTexture->GetWidth(), addTexture->GetHeight()), addEntityHover ? 1.f : 0.5f);
    position.y += font->GetHeight();
    
    for (Entity* entity : Hymn().activeScene.GetEntities()) {
        // Render background if selected.
        if (selectedEntity == entity) {
            color = glm::vec3(0.16078431372f, 0.15686274509f, 0.17647058823f);
            rectangle->Render(position, glm::vec2(size.x, font->GetHeight()), color);
        }
        
        font->RenderText(entity->name.c_str(), position + glm::vec2(20.f, 0.f), GetSize().x);
        position.y += font->GetHeight();
    }
    
    font->RenderText("Models", position, GetSize().x);
    addTexture->Render(position + glm::vec2(font->GetWidth("Models") + 5.f, 6.f), glm::vec2(addTexture->GetWidth(), addTexture->GetHeight()), addModelHover ? 1.f : 0.5f);
    position.y += font->GetHeight();
    
    for (Geometry::OBJModel* model : Hymn().models) {
        // Render background if selected.
        if (selectedModel == model) {
            color = glm::vec3(0.16078431372f, 0.15686274509f, 0.17647058823f);
            rectangle->Render(position, glm::vec2(size.x, font->GetHeight()), color);
        }
        
        font->RenderText(model->name.c_str(), position + glm::vec2(20.f, 0.f), GetSize().x);
        position.y += font->GetHeight();
    }
    
    font->RenderText("Textures", position, GetSize().x);
    addTexture->Render(position + glm::vec2(font->GetWidth("Textures") + 5.f, 6.f), glm::vec2(addTexture->GetWidth(), addTexture->GetHeight()), addTextureHover ? 1.f : 0.5f);
    position.y += font->GetHeight();
    
    for (Texture2D* texture : Hymn().textures) {
        // Render background if selected.
        if (selectedTexture == texture) {
            color = glm::vec3(0.16078431372f, 0.15686274509f, 0.17647058823f);
            rectangle->Render(position, glm::vec2(size.x, font->GetHeight()), color);
        }
        
        font->RenderText(texture->name.c_str(), position + glm::vec2(20.f, 0.f), GetSize().x);
        position.y += font->GetHeight();
    }
    
    font->RenderText("Sounds", position, GetSize().x);
    addTexture->Render(position + glm::vec2(font->GetWidth("Sounds") + 5.f, 6.f), glm::vec2(addTexture->GetWidth(), addTexture->GetHeight()), addSoundHover ? 1.f : 0.5f);
    position.y += font->GetHeight();
    
    for (Audio::SoundBuffer* sound : Hymn().sounds) {
        // Render background if selected.
        if (selectedSound == sound) {
            color = glm::vec3(0.16078431372f, 0.15686274509f, 0.17647058823f);
            rectangle->Render(position, glm::vec2(size.x, font->GetHeight()), color);
        }
        
        font->RenderText(sound->name.c_str(), position + glm::vec2(20.f, 0.f), GetSize().x);
        position.y += font->GetHeight();
    }
}

glm::vec2 ResourceList::GetSize() const {
    return size;
}

void ResourceList::SetSize(const glm::vec2& size) {
    this->size = size;
}

void ResourceList::SetEntitySelectedCallback(std::function<void(Entity*)> callback) {
    hasEntitySelectedCallback = true;
    entitySelectedCallback = callback;
}

void ResourceList::SetModelSelectedCallback(std::function<void(Geometry::OBJModel*)> callback) {
    hasModelSelectedCallback = true;
    modelSelectedCallback = callback;
}

void ResourceList::SetTextureSelectedCallback(std::function<void(Texture2D*)> callback) {
    hasTextureSelectedCallback = true;
    textureSelectedCallback = callback;
}

void ResourceList::SetSoundSelectedCallback(std::function<void(Audio::SoundBuffer*)> callback) {
    hasSoundSelectedCallback = true;
    soundSelectedCallback = callback;
}