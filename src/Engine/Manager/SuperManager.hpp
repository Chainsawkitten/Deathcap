#pragma once

#include <vector>
#include <map>
#include <typeinfo>

namespace Component {
    class SuperComponent;
}

/// %Component which all components inherit.
class SuperManager {
public:

    /// Destructor.
    virtual ~SuperManager();

    /// Add a component to the managers component list.
    /**
     * @param component The component to add.
     * @param componentType The type of the component to add.
     */
    void AddComponent(Component::SuperComponent* component, const std::type_info* componentType);

    /// Gets a list of the requested component type.
    /**
     * @return A list of pointers to all components of the world.
     */
    template<typename T> std::vector<T*>& GetComponents();

    /// Clears the component list.
    void ClearComponents();

    /// Clears the killed components from the list.
    void ClearKilledComponents();

protected:
    // Map containing list of components.
    std::map<const std::type_info*, std::vector<Component::SuperComponent*>> components;

};

template<typename T> inline std::vector<T*>& SuperManager::GetComponents() {
    return reinterpret_cast<std::vector<T*>&>(components[&typeid(T*)]);
}
