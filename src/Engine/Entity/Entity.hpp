#pragma once

#include <map>
#include <vector>
#include <typeinfo>
#include "../Entity/World.hpp"
#include <json/json.h>
#include "../Component/SuperComponent.hpp"

#include "../Manager/Managers.hpp"

/// %Entity containing various components.
class Entity {
    public:
        /// Create new entity.
        /**
         * @param world The game world in which the entity is contained.
         * @param name Name of the entity.
         */
        Entity(World* world, const std::string& name);
        
        /// Destructor.
        ~Entity();
        
        /// Get the entity's parent entity.
        /**
         * @return The parent entity, or nullptr if none.
         */
        Entity* GetParent() const;
        
        /// Add child entity.
        /**
        * @param name The name of the child entity.
        * @return The new entity.
        */
        Entity* AddChild(const std::string& name = "");

        /// Remove child entity.
        /**
        * @param child The entity you want to remove.
        * @return Did we manage to remove the child.
        */
        bool RemoveChild(Entity* child);
        
        /// Set a new parent.
        /**
        * @param newParent The entity you want to be the new parent.
        * @return Did we manage set the new parent?.
        */
        bool SetParent(Entity* newParent);

        /// Check if entity is a child.
        /**
        * @param child The entity you want to check if it is a child.
        * @param deep True if we want to check if it's a grandchild, false if we do not.
        * @return True if it has a child, false if it does not.
        */
        bool HasChild(const Entity* child, bool deep = true) const;

        /// Instantiate a scene as a child to this entity.
        /**
         * @param name The name of the scene to instantiate.
         * @return The created root entity of the scene.
         */
        Entity* InstantiateScene(const std::string& name);
        
        /// Get all of the entity's children.
        /**
         * @return All the children.
         */
        const std::vector<Entity*>& GetChildren() const;
        
        /// Get child based on its name.
        /**
         * @param name The name of the child to get.
         * @return The child or nullptr if none was found.
         */
        Entity* GetChild(const std::string& name) const;
        
        /// Get whether the entity is an instantiated scene.
        /**
         * @return Whether the entity is an instantiated scene.
         */
        bool IsScene() const;
        
        /// Adds component with type T.
        /**
         * @return The created component.
         */
        template<typename T> T* AddComponent();
        
        /// Gets component with type T.
        /**
         * @return The requested component (or nullptr).
         */
        template<typename T> T* GetComponent() const;
        
        /// Kill component of type T.
        template <typename T> void KillComponent();
        
        /// Kill the entity, will be removed at the end of the frame.
        void Kill();
        
        /// Get whether entity has been killed.
        /**
         * @return Whether the entity has been killed.
         */
        bool IsKilled() const;
        
        /// Save the entity.
        /**
         * @return JSON value to be stored on disk.
         */
        Json::Value Save() const;
        
        /// Load entity from JSON node.
        /**
         * @param node JSON node to load from.
         */
        void Load(const Json::Value& node);
        
        /// Get the model matrix.
        /**
         * @return The model matrix.
         */
        glm::mat4 GetModelMatrix() const;

        /// Get the local model matrix.
        /**
        * @return The local model matrix.
        */
        glm::mat4 GetLocalMatrix() const;
        
        /// Get orientation matrix.
        /**
         * @return The entity's orientation matrix.
         */
        glm::mat4 GetOrientation() const;
        
        /// Get orientation matrix (for camera).
        /**
         * Calculates the orientation matrix as if the entity was a camera.
         * @return The entity's orientation matrix.
         */
        glm::mat4 GetCameraOrientation() const;
        
        /// Get direction of the entity.
        /**
         * @return The entity's direction.
         */
        glm::vec3 GetDirection() const;
        
        /// Get the position in the world.
        /**
         * @return The position in the world (not relative to parent).
         */
        glm::vec3 GetWorldPosition() const;
        
        /// Name of the entity.
        std::string name;
        
        /// Position relative to the parent entity.
        /**
         * Default: 0.f, 0.f, 0.f
         */
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        
        /// Scale.
        /**
         * Default: 1.f, 1.f, 1.f
         */
        glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
        
        /// Rotation (pitch, yaw, roll in degrees).
        /**
         * Default: 0.f, 0.f, 0.f
         */
        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);

        /// Get the entity's UID
        /**
         * @return The entity's UID
         */
        unsigned int GetUniqueIdentifier() const;
           
        /// Set the entity's UID
        /**
         * @param UID the entity's unique identifier to be set
         */
        void SetUniqueIdentifier(unsigned int UID);

        /// Whether the entity is active.
        bool enabled = true;
        
    private:
        template<typename T> void Save(Json::Value& node, const std::string& name) const;
        template<typename T> void Load(const Json::Value& node, const std::string& name);
        Component::SuperComponent* AddComponent(const std::type_info* componentType);
        void LoadComponent(const std::type_info* componentType, const Json::Value& node);
        void KillHelper();
        
        World* world;
        Entity* parent = nullptr;
        std::vector<Entity*> children;
        bool scene = false;
        std::string sceneName;
        
        std::map<const std::type_info*, Component::SuperComponent*> components;
        
        bool killed = false;
        unsigned int uniqueIdentifier = 0;
};

template<typename T> T* Entity::AddComponent() {
    const std::type_info* componentType = &typeid(T*);
    if (components.find(componentType) != components.end())
        return nullptr;
    
    return static_cast<T*>(AddComponent(componentType));
}

template<typename T> T* Entity::GetComponent() const {
    auto it = components.find(&typeid(T*));
    if (it != components.end())
        return static_cast<T*>(it->second);
    else
        return nullptr;
}

template <typename T> void Entity::KillComponent() {
    const std::type_info* componentType = &typeid(T*);
    if (components.find(componentType) != components.end()) {
        components[componentType]->Kill();
        components.erase(componentType);
    }
}

template<typename T> void Entity::Save(Json::Value& node, const std::string& name) const {
    auto it = components.find(&typeid(T*));
    if (it != components.end())
        node[name] = it->second->Save();
}

template<typename T> void Entity::Load(const Json::Value& node, const std::string& name) {
    Json::Value componentNode = node[name];
    if (!componentNode.isNull()) {
        const std::type_info* componentType = &typeid(T*);
        LoadComponent(componentType, componentNode);
    }
}
