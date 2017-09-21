#pragma once

#include <vector>
#include <map>
#include <typeinfo>

namespace Component {
    class SuperComponent;
}

class World;

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
    void AddComponent(World* world, Component::SuperComponent* component, const std::type_info* componentType);

    /// Gets a list of the requested component type.
    /**
     * @return A list of pointers to all components of the world.
     */
    template<typename T> std::vector<T*>& GetComponents(World* world);

    /// Clears the components of a certain world.
    void ClearComponents(World* world);

    /// Clears the entire component list.
    void ClearAllComponents();

    /// Clears the killed components from the list.
    void ClearKilledComponents(World* world);

protected:
    // Map containing list of components.
    std::map<const std::type_info*, std::vector<Component::SuperComponent*>> components;

    // Map containing list of components.
    std::map<World*, std::map<const std::type_info*, std::vector<Component::SuperComponent*>>> worldComponents;

};

template<typename T> inline std::vector<T*>& SuperManager::GetComponents(World* world) {

    // The only reason we have two worlds is because we want a editor camera, and that camera is supposed to get components from another world. This is wierd.
    //std::map<const std::type_info*, std::vector<Component::SuperComponent*>> componentMap = worldComponents[world];
    
    return reinterpret_cast<std::vector<T*>&>(components[&typeid(T*)]);

}
