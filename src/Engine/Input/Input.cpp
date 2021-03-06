#include "Input.hpp"

#include <cstring>
#include "../Component/VRDevice.hpp"
#include <GLFW/glfw3.h> // Must be included at the end to make sure gl.h is included AFTER glew.h

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

Input& Input::GetInstance() {
    static Input instance;
    return instance;
}

void Input::SetWindow(GLFWwindow* window) {
    this->window = window;
}

bool Input::CheckButton(int index) const{
    Button* button = buttons[index];
    int state;
    if (button->key == 420)
        state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
    else
        state = glfwGetKey(window, button->key);
    return state == button->state;
}

bool Input::CheckVRButton(int index, Component::VRDevice *controller) const{
    if (controller != nullptr) {
        Button* button = buttons[index];
        return controller->HandleInput(button->key);
    }
    return false;
}

Json::Value Input::Save() const{
    Json::Value buttonsNode;
    
    buttonsNode["size"] = buttons.size();
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        buttonsNode[std::to_string(i)]["action"] = buttons[i]->action;
        buttonsNode[std::to_string(i)]["key"] = buttons[i]->key;
        buttonsNode[std::to_string(i)]["state"] = buttons[i]->state;
    }
    
    return buttonsNode;
}

void Input::Load(const Json::Value& buttonsNode) {
    buttons.clear();
    
    unsigned int size = buttonsNode.get("size", 0).asUInt();
    for (unsigned int i = 0; i < size; ++i) {
        Button* button = new Button();
        
        strcpy(button->action, buttonsNode[std::to_string(i)]["action"].asCString());
        button->key = buttonsNode[std::to_string(i)]["key"].asInt();
        button->state = buttonsNode[std::to_string(i)]["state"].asInt();
        
        buttons.push_back(button);
    }
}
