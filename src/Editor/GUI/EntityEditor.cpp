#include "EntityEditor.hpp"

#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>
#include <Engine/Geometry/Rectangle.hpp>
#include "TransformEditor.hpp"

using namespace GUI;

EntityEditor::EntityEditor(Widget* parent) : Widget(parent) {
    rectangle = Managers().resourceManager->CreateRectangle();
    entity = nullptr;
    transformEditor = new TransformEditor(this);
}

EntityEditor::~EntityEditor() {
    Managers().resourceManager->FreeRectangle();
    delete transformEditor;
}

void EntityEditor::Update() {
    transformEditor->Update();
}

void EntityEditor::Render(const glm::vec2& screenSize) {
    glm::vec3 color(0.06666666666f, 0.06274509803f, 0.08235294117f);
    rectangle->Render(GetPosition(), size, color, screenSize);
    
    transformEditor->Render(screenSize);
}

void EntityEditor::SetPosition(const glm::vec2& position) {
    Widget::SetPosition(position);
    
    transformEditor->SetPosition(position);
}

glm::vec2 EntityEditor::GetSize() const {
    return size;
}

void EntityEditor::SetSize(const glm::vec2& size) {
    this->size = size;
}

void EntityEditor::SetEntity(Entity* entity) {
    this->entity = entity;
    
    transformEditor->SetEntity(entity);
}
