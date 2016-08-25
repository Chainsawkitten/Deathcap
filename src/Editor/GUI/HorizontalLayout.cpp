#include "HorizontalLayout.hpp"

#include <Engine/Geometry/Rectangle.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>

using namespace GUI;

HorizontalLayout::HorizontalLayout(Widget* parent) : Container(parent) {
    rectangle = Managers().resourceManager->CreateRectangle();
}

HorizontalLayout::~HorizontalLayout() {
    Managers().resourceManager->FreeRectangle();
}

void HorizontalLayout::Render() {
    // Set color.
    glm::vec3 color(0.06666666666f, 0.06274509803f, 0.08235294117f);
    rectangle->Render(GetPosition(), size, color);
    
    RenderWidgets();
}

void HorizontalLayout::AddWidget(Widget* widget) {
    Container::AddWidget(widget);
    widget->SetPosition(GetPosition() + nextPosition);
    nextPosition.x += widget->GetSize().x;
}

void HorizontalLayout::ClearWidgets() {
    Container::ClearWidgets();
    nextPosition = glm::vec2(0.f, 0.f);
}

glm::vec2 HorizontalLayout::GetSize() const {
    return this->size;
}

void HorizontalLayout::SetSize(const glm::vec2& size) {
    this->size = size;
}