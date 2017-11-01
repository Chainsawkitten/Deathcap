#include "AnimationControllerEditor.hpp"
#include <Engine/Animation/AnimationController.hpp>
#include <Engine/Animation/AnimationClip.hpp>
#include <Utility/Log.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>
#include <Engine/Hymn.hpp>

using namespace GUI;

AnimationControllerEditor::AnimationControllerEditor() {
}

AnimationControllerEditor::~AnimationControllerEditor() {
}

void AnimationControllerEditor::SetAnimationController(Animation::AnimationController* animationController) {
    if (this->animationController != nullptr)
        this->animationController->Save(Hymn().GetPath() + "/" + animationController->path + animationController->name + ".asset");

    this->animationController = animationController;
}

Animation::AnimationController* AnimationControllerEditor::GetAnimationController() {
    return animationController;
}

void AnimationControllerEditor::SetVisible(bool visible) {
    NodeEditor::SetVisible(visible);
    if (this->animationController != nullptr)
        animationController->Save(Hymn().GetPath() + "/" + animationController->path + animationController->name + ".asset");
}

void AnimationControllerEditor::ShowContextMenu() {
    if (animationController == nullptr) {
        Log() << "ERROR: No animation controller choosen.\n";
        return;
    }

    if (ImGui::MenuItem("Add animation action")) {
        Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
        std::string name = "Action #" + std::to_string(animationController->animationNodes.size() + 1);
        unsigned int size = name.size() < 127 ? name.size() + 1 : 128;
        memcpy(newAction->name, name.c_str(), size);
        newAction->index = animationController->animationNodes.size();
        animationController->animationNodes.push_back(newAction);
    }

    if (ImGui::MenuItem("Add animation transition")) {
        Animation::AnimationController::AnimationTransition* newTransition = new Animation::AnimationController::AnimationTransition;
        std::string name = "Animation transition: " + std::to_string(animationController->animationNodes.size() + 1);
        unsigned int size = name.size() < 127 ? name.size() + 1 : 128;
        memcpy(newTransition->name, name.c_str(), size);
        animationController->animationNodes.push_back(newTransition);
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add bool")) {
        Animation::AnimationController::BoolItem* newBool = new Animation::AnimationController::BoolItem;
        newBool->name.append(std::to_string(animationController->boolMap.size() + 1));
        animationController->boolMap.push_back(newBool);
    }

    if (ImGui::MenuItem("Add float")) {
        Animation::AnimationController::FloatItem* newFloat = new Animation::AnimationController::FloatItem;
        newFloat->name.append(std::to_string(animationController->floatMap.size() + 1));
        animationController->floatMap.push_back(newFloat);
    }
}

void AnimationControllerEditor::ShowNode(Node* node) {
    if (dynamic_cast<Animation::AnimationController::AnimationAction*>(node) != nullptr) {
        ImGui::Text("Action: %s", node->name);
        ImGui::InputText("Name", node->name, 128);

        // Dynamic cast to AnimationAction.
        Animation::AnimationController::AnimationAction* action = dynamic_cast<Animation::AnimationController::AnimationAction*>(node);
        
        // Select animation clip.
        if (ImGui::Button("Select animation clip##Clip"))
            ImGui::OpenPopup("Select animation clip##Clip");

        // Playback modifier.
        ImGui::Text("Playback modifier");
        if (ImGui::RadioButton("Static", action->isPlaybackModifierStatic))
            action->isPlaybackModifierStatic = true;

        // Keep the radiobuttons on the same line.
        ImGui::SameLine();
        
        if (ImGui::RadioButton("Dynamic", !action->isPlaybackModifierStatic))
            action->isPlaybackModifierStatic = false;

        // If static modifier select at static float value.
        if (action->isPlaybackModifierStatic)
            ImGui::DragFloat("", &action->playbackModifier, 0.01f);
        else {
            if (ImGui::Button("Select float##Float"))
                ImGui::OpenPopup("Select float##Float");
        }

        // Repeat.
        ImGui::Checkbox("Repeat", &action->repeat);

        // Open popup.
        if (ImGui::BeginPopup("Select animation clip##Clip")) {
            ImGui::Text("Select animation");
            ImGui::Separator();

            if (resourceSelector.Show(ResourceList::Resource::Type::ANIMATION_CLIP)) {
                // If clip is attached to action free it.
                if (action->animationClip != nullptr) {
                    Managers().resourceManager->FreeAnimationClip(action->animationClip);
                    action->animationClip = nullptr;
                }

                std::string path = resourceSelector.GetSelectedResource().GetPath();
                action->animationClip = Managers().resourceManager->CreateAnimationClip(path);
                if (path.size() < 512) {
                    memcpy(action->animationClipName, path.c_str(), path.size() + 1);
                } else {
                    Log() << "Error: " << (int)path.size() << "is to long.\n";
                }
            }

            ImGui::EndPopup();
        }
    } else if (dynamic_cast<Animation::AnimationController::AnimationTransition*>(node) != nullptr) {
        ImGui::Text("Transition: %s", node->name);
        ImGui::InputText("Name", node->name, 128);

    }
}

void GUI::AnimationControllerEditor::ShowValues() {
    ImGui::Text("Bools");
    ImGui::Separator();

    for (unsigned int i = 0; i < animationController->boolMap.size(); ++i) {
        Animation::AnimationController::BoolItem* item = animationController->boolMap[i];
        ImGui::Text("Bool: %s", item->name.c_str());
        ImGui::Checkbox("Value", &item->value);
    }

    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Text("Floats");
    ImGui::Separator();

    for (unsigned int i = 0; i < animationController->floatMap.size(); ++i) {
        ImGui::BeginChild(animationController->floatMap[i]->name.c_str(), ImVec2(0, 50), true);
        ImGui::Text("Float: %s", animationController->floatMap[i]->name.c_str());
        ImGui::DragFloat("Value", &animationController->floatMap[i]->value, 0.01f);
        ImGui::EndChild();
    }
}

Node** AnimationControllerEditor::GetNodeArray() {
    return &animationController->animationNodes[0];
}

unsigned int AnimationControllerEditor::GetNumNodes() {
    if (animationController == nullptr) {
        Log() << "ERROR: No animation controller choosen.\n";
        return 0;
    }

    return animationController->animationNodes.size();
}

bool AnimationControllerEditor::CanConnect(Node* output, Node* input) {
    if (typeid(*output) == typeid(Animation::AnimationController::AnimationAction)) {
        if (typeid(*input) == typeid(Animation::AnimationController::AnimationTransition))
            return true;

        return false;
    } else if (typeid(*output) == typeid(Animation::AnimationController::AnimationTransition)) {
        if (typeid(*input) == typeid(Animation::AnimationController::AnimationAction))
            return true;

        return false;
    }

    return false;
}
