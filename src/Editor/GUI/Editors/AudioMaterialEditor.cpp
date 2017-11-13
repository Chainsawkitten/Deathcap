#include "AudioMaterialEditor.hpp"

#include <Engine/Audio/AudioMaterial.hpp>
#include "../FileSelector.hpp"
#include <functional>
#include <imgui.h>
#include "ImGui/GuiHelpers.hpp"
#include <Engine/Hymn.hpp>

using namespace GUI;

AudioMaterialEditor::AudioMaterialEditor() {
    name[0] = '\0';
}

void AudioMaterialEditor::Show() {
    if (ImGui::Begin(("Sound: " + audioMaterial->name + "###" + std::to_string(reinterpret_cast<uintptr_t>(audioMaterial))).c_str(), &visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders)) {
        if (ImGui::InputText("Name", name, 128)) {
            // Rename audio material file.
            std::string path = Hymn().GetPath() + "/" + audioMaterial->path;
            rename((path + audioMaterial->name + ".json").c_str(), (path + name + ".json").c_str());
            
            audioMaterial->name = name;
        }
        
        glm::vec3 freqAbsorption(audioMaterial->lowFreqAbsorption, audioMaterial->midFreqAbsorption, audioMaterial->highFreqAbsorption);
        ImGui::Text("Frequency absorption");
        ImGui::ShowHelpMarker("Fraction of sound energy absorbed at [low, mid, high] frequencies", 150.f);
        ImGui::Indent();
        ImGui::DragFloat3("[low, mid, high] absorption", &freqAbsorption[0], 0.f, 0.f, 1.f);
        ImGui::Unindent();
        audioMaterial->lowFreqAbsorption = freqAbsorption[0];
        audioMaterial->midFreqAbsorption = freqAbsorption[1];
        audioMaterial->highFreqAbsorption = freqAbsorption[2];

        glm::vec3 freqTransmission(audioMaterial->lowFreqTransmission, audioMaterial->midFreqTransmission, audioMaterial->highFreqTransmission);
        ImGui::Text("Frequency transmission");
        ImGui::ShowHelpMarker("Fraction of sound energy transmitted through at [low, mid, high] frequencies", 165.f);
        ImGui::Indent();
        ImGui::DragFloat3("[low, mid, high] transmitted", &freqTransmission[0], 0.f, 0.f, 1.f);
        ImGui::Unindent();
        audioMaterial->lowFreqTransmission = freqTransmission[0];
        audioMaterial->midFreqTransmission = freqTransmission[1];
        audioMaterial->highFreqTransmission = freqTransmission[2];

        ImGui::Text("Scattering");
        ImGui::ShowHelpMarker("Fraction of sound energy that is scattered in a random direction when it reaches the surface", 80.f);
        ImGui::ShowHelpMarker("A value of 0.0 describes a smooth surface with mirror - like reflection properties; a value of 1.0 describes rough surface with diffuse reflection properties.", 100.f);
        ImGui::Indent();
        ImGui::DraggableFloat("", audioMaterial->scattering, 0.f, 1.f);
        ImGui::Unindent();
    }
    ImGui::End();
    
    if (fileSelector.IsVisible())
        fileSelector.Show();
}

const Audio::AudioMaterial* AudioMaterialEditor::GetAudioMaterial() const {
    return audioMaterial;
}

void AudioMaterialEditor::SetAudioMaterial(Audio::AudioMaterial* audioMaterial) {
    this->audioMaterial = audioMaterial;
    
    strcpy(name, audioMaterial->name.c_str());
}

bool AudioMaterialEditor::IsVisible() const {
    return visible;
}

void AudioMaterialEditor::SetVisible(bool visible) {
    this->visible = visible;
}
