#include "TextureEditor.hpp"

#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>
#include <Engine/Geometry/Rectangle.hpp>
#include <Engine/Texture/Texture2D.hpp>
#include "Subtract.png.hpp"
#include <Engine/Font/Font.hpp>
#include "ABeeZee.ttf.hpp"
#include "../Label.hpp"
#include "StringEditor.hpp"
#include "BoolEditor.hpp"
#include "../TextButton.hpp"
#include "../FileSelector.hpp"
#include <functional>
#include <Engine/Hymn.hpp>
#include <Engine/Util/FileSystem.hpp>
#include "../ImageTextButton.hpp"
#include <imgui.h>

using namespace GUI;

TextureEditor::TextureEditor(FileSelector* fileSelector) {
    this->fileSelector = fileSelector;
}

TextureEditor::~TextureEditor() {
    
}

void TextureEditor::Show() {
    if (ImGui::Begin("Texture editor"), &visible) {
        ImGui::InputText("Name", name, 128);
        texture->name = name;
        
        if (ImGui::Button("Load PNG image")) {
            LoadPressed();
        }
        
        ImGui::Checkbox("SRGB", &texture->srgb);
    }
    ImGui::End();
}

void TextureEditor::SetTexture(Texture2D* texture) {
    this->texture = texture;
    
    strcpy(name, texture->name.c_str());
}

bool TextureEditor::IsVisible() const {
    return visible;
}

void TextureEditor::SetVisible(bool visible) {
    this->visible = visible;
}

void TextureEditor::LoadPressed() {
    fileSelector->SetExtension("png");
    fileSelector->SetFileSelectedCallback(std::bind(&FileSelected, this, std::placeholders::_1));
    fileSelector->SetVisible(true);
}

void TextureEditor::FileSelected(const std::string& file) {
    std::string destination = Hymn().GetPath() + FileSystem::DELIMITER + "Textures" + FileSystem::DELIMITER + texture->name + ".png";
    FileSystem::Copy(file.c_str(), destination.c_str());
    texture->Load(file.c_str(), texture->srgb);
}
