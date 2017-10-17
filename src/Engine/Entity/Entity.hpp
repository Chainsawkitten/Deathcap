#pragma once

#include <map>
#include <vector>
#include <typeinfo>
#include "../Entity/World.hpp"
#include <json/json.h>
#include "../Component/SuperComponent.hpp"
#include <fstream>
#include "../linking.hpp"

/// %Entity containing various components.
class Entity {
    public:
        /// Create new entity.
        /**
         * @param world The game world in which the entity is contained.
         * @param name Name of the entity.
         */
        ENGINE_EXPORT Entity(World* world, const std::string& name);
        
        /// Destructor.
        ENGINE_EXPORT ~Entity();
        
        /// Get the entity's parent entity.
        /**
         * @return The parent entity, or nullptr if none.
         */
        ENGINE_EXPORT Entity* GetParent() const;
        
        /// Add child entity.
        /**
        * @param name The name of the child entity.
        * @return The new entity.
        */
        ENGINE_EXPORT Entity* AddChild(const std::string& name = "");

        /// Remove child entity.
        /**
        * @param child The entity you want to remove.
        * @return Did we manage to remove the child.
        */
        ENGINE_EXPORT bool RemoveChild(Entity* child);
        
        /// Set a new parent.
        /**
        * @param newParent The entity you want to be the new parent.
        * @return Did we manage set the new parent?.
        */
        ENGINE_EXPORT bool SetParent(Entity* newParent);

        /// Check if entity is a child.
        /**
         * @param child The entity you want to check if it is a child.
         * @param deep True if we want to check if it's a grandchild, false if we do not.
         * @return True if it has a child, false if it does not.
         */
        ENGINE_EXPORT bool HasChild(const Entity* child, bool deep = true) const;

        /// Instantiate a scene as a child to this entity.
        /**
         * @param name The name of the scene to instantiate.
         * @return The created root entity of the scene.
         */
        ENGINE_EXPORT Entity* InstantiateScene(const std::string& name, const std::string& originScene);
        
        /// Check if scene already exists in any of json files.
        /**
        * @param filename The name of the scene to check.
        * @param error Set to true inside the function if it allready exists.
        * @param originScene Name of scene you want to check.
        * @param root The json value of root scene.
        */
        ENGINE_EXPORT void CheckIfSceneExists(const std::string& filename, bool & error, const std::string& originScene, Json::Value root);

        /// Get all of the entity's children.
        /**
         * @return All the children.
         */
        ENGINE_EXPORT const std::vector<Entity*>& GetChildren() const;
        
        /// Get child based on its name.
        /**
         * @param name The name of the child to get.
         * @return The child or nullptr if none was found.
         */
        ENGINE_EXPORT Entity* GetChild(const std::string& name) const;
        
        /// Get whether the entity is an instantiated scene.
        /**
         * @return Whether the entity is an instantiated scene.
         */
        ENGINE_EXPORT bool IsScene() const;
        
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
        ENGINE_EXPORT void Kill();
        
        /// Get whether entity has been killed.
        /**
         * @return Whether the entity has been killed.
         */
        ENGINE_EXPORT bool IsKilled() const;
        
        /// Save the entity.
        /**
         * @return JSON value to be stored on disk.
         */
        ENGINE_EXPORT Json::Value Save() const;
        
        /// Load entity from JSON node.
        /**
         * @param node JSON node to load from.
         */
        ENGINE_EXPORT void Load(const Json::Value& node);
        
        /// Get the model matrix.
        /**
         * @return The model matrix.
         */
        ENGINE_EXPORT glm::mat4 GetModelMatrix() const;

        /// Get the local model matrix.
        /**
         * @return The local model matrix.
         */
        ENGINE_EXPORT glm::mat4 GetLocalMatrix() const;
        
        /// Get orientation matrix.
        /**
         * @return The entity's orientation matrix.
         */
        ENGINE_EXPORT glm::mat4 GetOrientation() const;
        
        /// Get orientation matrix (for camera).
        /**
         * Calculates the orientation matrix as if the entity was a camera.
         * @return The entity's orientation matrix.
         */
        ENGINE_EXPORT glm::mat4 GetCameraOrientation() const;
        
        /// Get direction of the entity.
        /**
         * @return The entity's direction.
         */
        ENGINE_EXPORT glm::vec3 GetDirection() const;
        
        /// Get the position in the world.
        /**
         * @return The position in the world (not relative to parent).
         */
        ENGINE_EXPORT glm::vec3 GetWorldPosition() const;
        
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
        
        /// Rotation (yaw, pitch, roll in degrees).
        /**
         * Default: 0.f, 0.f, 0.f
         */
        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);

        /// Get the entity's UID
        /**
         * @return The entity's UID
         */
        ENGINE_EXPORT unsigned int GetUniqueIdentifier() const;
           
        /// Set the entity's UID
        /**
         * @param UID the entity's unique identifier to be set
         */
        ENGINE_EXPORT void SetUniqueIdentifier(unsigned int UID);

        /// Whether the entity is active.
        bool enabled = true;

        /// Whether the entity is static.
        bool isStatic = false;
        
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
