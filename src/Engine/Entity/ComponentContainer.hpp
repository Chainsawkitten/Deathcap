#pragma once

#include <vector>

/// Stores components.
template<class C> class ComponentContainer {
    public:
        /// Create new component container.
        ComponentContainer();
        
        /// Destructor.
        ~ComponentContainer();
        
        /// Create a new component.
        /**
         * @return The created component.
         */
        C* Add();
        
        /// Clear all killed components.
        void ClearKilled();
        
        /// Get all components.
        /**
         * @return All of the components in the container.
         */
        const std::vector<C*>& GetAll() const;
        
    private:
        std::vector<C*> components;
};

template<class C> ComponentContainer<C>::ComponentContainer() {
    
}

template<class C> ComponentContainer<C>::~ComponentContainer() {
    for (C* component : components)
        delete component;
}

template<class C> C* ComponentContainer<C>::Add() {
    C* component = new C();
    components.push_back(component);
    return component;
}

template<class C> void ComponentContainer<C>::ClearKilled() {
    // Clear killed components.
    std::size_t i = 0;
    while (i < components.size()) {
        if (components[i]->IsKilled()) {
            delete components[i];
            components[i] = components[components.size() - 1];
            components.pop_back();
        } else {
            ++i;
        }
    }
}

template<class C> const std::vector<C*>& ComponentContainer<C>::GetAll() const {
    return components;
}
