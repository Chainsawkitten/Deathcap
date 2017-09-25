#include "SuperManager.hpp"
#include "../Component/SuperComponent.hpp"

SuperManager::~SuperManager() {

}

void SuperManager::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    components[componentType].push_back(component);
}

void SuperManager::ClearComponents() {
    for (auto& it : components) {
        for (Component::SuperComponent* component : it.second)
            delete component;
    }
    components.clear();
}

void SuperManager::ClearKilledComponents() {
    // Clear killed components.
    std::size_t i;
    for (auto& componentIt : components) {
        i = 0;
        while (i < componentIt.second.size()) {
            if (componentIt.second[i]->IsKilled()) {
                delete componentIt.second[i];
                componentIt.second[i] = componentIt.second[componentIt.second.size() - 1];
                componentIt.second.pop_back();
            }
            else {
                ++i;
            }
        }
    }
}
