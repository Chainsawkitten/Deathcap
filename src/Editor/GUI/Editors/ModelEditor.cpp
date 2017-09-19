#include "ModelEditor.hpp"
#include <Engine/Geometry/Model.hpp>
#include "../FileSelector.hpp"
#include <functional>
#include <Engine/Hymn.hpp>
#include <Engine/Util/FileSystem.hpp>
#include <imgui.h>
#include "Util/AssetMetaData.hpp"
#include <Utility/Log.hpp>

using namespace GUI;

void ModelEditor::Show() {
    if (ImGui::Begin(("Model: " + model->name + "###" + std::to_string(reinterpret_cast<uintptr_t>(model))).c_str(), &visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders)) {
        ImGui::InputText("Name", name, 128);
        model->name = name;

        if (ImGui::Button("Open model")) {
            fileSelector.AddExtensions("fbx");

            // Set the initial path to the models directory.
            fileSelector.SetInitialPath((Hymn().GetPath() + FileSystem::DELIMITER + "Models").c_str());
            fileSelector.SetFileSelectedCallback(std::bind(&ModelEditor::FileSelected, this, std::placeholders::_1));
            fileSelector.SetVisible(true);
        }

        if (hasSourceFile) {
            ImGui::Text("Mesh Data");
            ImGui::Checkbox("Triangulate", &triangulate);
            ImGui::Checkbox("Import Normals", &importNormals);
            ImGui::Checkbox("Import Tangents", &importTangents);

            std::string button = isImported ? "Re-import" : "Import";

            if (ImGui::Button(button.c_str())) {
                AssetConverter asset;
                asset.Convert(source.c_str(), (destination + ".asset").c_str(), triangulate, importNormals, importTangents);
                model->Load(destination.c_str());
                msgString = asset.Success() ? "Success\n" : asset.GetErrorString();
                isImported = true;

                AssetMetaData metaData;
                AssetMetaData::MeshImportData * importData = new AssetMetaData::MeshImportData;
                importData->triangulate = triangulate;
                importData->importNormals = importNormals;
                importData->importTangents = importTangents;
                metaData.GenerateMetaData((destination + ".asset.meta").c_str(), importData);

                // Check if file exits, if it doesn't copy the fbx file to the hymn folder.
                if (!FileSystem::FileExists((destination + "." + FileSystem::GetExtension(source)).c_str())) {
                    FileSystem::Copy(source.c_str(), (destination + "." + FileSystem::GetExtension(source)).c_str());
                }

                delete importData;
                importData = nullptr;
            }

            if (isImported)
                ImGui::Text(msgString.c_str());
        }
    }
    ImGui::End();

    if (fileSelector.IsVisible())
        fileSelector.Show();
}

const Geometry::Model* ModelEditor::GetModel() const {
    return model;
}

void ModelEditor::SetModel(Geometry::Model* model) {
    this->model = model;

    strcpy(name, model->name.c_str());
}

bool ModelEditor::IsVisible() const {
    return visible;
}

void ModelEditor::SetVisible(bool visible) {
    this->visible = visible;
}

void ModelEditor::FileSelected(const std::string& file) {
    destination = Hymn().GetPath() + FileSystem::DELIMITER + "Models" + FileSystem::DELIMITER + model->name;
    source = file;
    //model->name = FileSystem::GetName(source);
    hasSourceFile = true;
    Log() << FileSystem::GetName(source);
    std::string filePath(destination + ".asset.meta");
    if (FileSystem::FileExists(filePath.c_str())) {

        AssetMetaData metaData;
        AssetMetaData::MeshImportData * importData = metaData.GetMetaData(filePath.c_str());
        triangulate = importData->triangulate;
        importNormals = importData->importNormals;
        importTangents = importData->importTangents;
        delete importData;
        
        isImported = true;
    }
}
