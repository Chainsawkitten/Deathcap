#pragma once

#include <map>
#include <vector>
#include <typeindex>
#include "../Entity/World.hpp"
#include <json/json.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
        ENGINE_API Entity(World* world, const std::string& name);
        
        /// Destructor.
        ENGINE_API ~Entity();
        
        /// Get the entity's parent entity.
        /**
         * @return The parent entity, or nullptr if none.
         */
        ENGINE_API Entity* GetParent() const;
        
        /// Add child entity.
        /**
        * @param name The name of the child entity.
        * @return The new entity.
        */
        ENGINE_API Entity* AddChild(const std::string& name = "");

        /// Remove child entity.
        /**
        * @param child The entity you want to remove.
        * @return Did we manage to remove the child.
        */
        ENGINE_API bool RemoveChild(Entity* child);
        
        /// Set a new parent.
        /**
        * @param newParent The entity you want to be the new parent.
        * @return Did we manage set the new parent?.
        */
        ENGINE_API bool SetParent(Entity* newParent);

        /// Check if entity is a child.
        /**
         * @param child The entity you want to check if it is a child.
         * @param deep True if we want to check if it's a grandchild, false if we do not.
         * @return True if it has a child, false if it does not.
         */
        ENGINE_API bool HasChild(const Entity* child, bool deep = true) const;

        /// Instantiate a scene as a child to this entity.
        /**
         * @param name The name of the scene to instantiate.
         * @return The created root entity of the scene.
         */
        ENGINE_API Entity* InstantiateScene(const std::string& name, const std::string& originScene);
        
        /// Check if scene already exists in any of json files.
        /**
        * @param filename The name of the scene to check.
        * @param error Set to true inside the function if it allready exists.
        * @param originScene Name of scene you want to check.
        * @param root The json value of root scene.
        */
        ENGINE_API void CheckIfSceneExists(const std::string& filename, bool & error, const std::string& originScene, Json::Value root);

        /// Get all of the entity's children.
        /**
         * @return All the children.
         */
        ENGINE_API const std::vector<Entity*>& GetChildren() const;
        
        /// Get child based on its name.
        /**
         * @param name The name of the child to get.
         * @return The child or nullptr if none was found.
         */
        ENGINE_API Entity* GetChild(const std::string& name) const;
        
        /// Get whether the entity is an instantiated scene.
        /**
         * @return Whether the entity is an instantiated scene.
         */
        ENGINE_API bool IsScene() const;
        
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
        ENGINE_API void Kill();
        
        /// Get whether entity has been killed.
        /**
         * @return Whether the entity has been killed.
         */
        ENGINE_API bool IsKilled() const;
        
        /// Save the entity.
        /**
         * @return JSON value to be stored on disk.
         */
        ENGINE_API Json::Value Save() const;
        
        /// Load entity from JSON node.
        /**
         * @param node JSON node to load from.
         */
        ENGINE_API void Load(const Json::Value& node);
        
        /// Get the model matrix.
        /**
         * @return The model matrix.
         */
        ENGINE_API glm::mat4 GetModelMatrix() const;

        /// Get the local model matrix.
        /**
         * @return The local model matrix.
         */
        ENGINE_API glm::mat4 GetLocalMatrix() const;

        /// Get the rotation of this entity.
        /**
         * @return The rotation in local space.
         */
        ENGINE_API glm::quat GetLocalOrientation() const;
        
        /// Get orientation of the entity.
        /**
         * @return The rotation in world space.
         */
        ENGINE_API glm::quat GetWorldOrientation() const;
        
        /// Get direction of the entity.
        /**
         * @return The entity's direction.
         */
        ENGINE_API glm::vec3 GetDirection() const;
        
        /// Get the position in the world.
        /**
         * @return The position in the world (not relative to parent).
         */
        ENGINE_API glm::vec3 GetWorldPosition() const;

        /// Set the position of the entity in world space.
        /**
         * @param worldPos The world position you want the entity to have.
         */
        ENGINE_API void SetWorldPosition(const glm::vec3& worldPos);

        /// Set the orientation of the entity in world space.
        /**
         * @param worldRot New orientation.
         */
        ENGINE_API void SetWorldOrientation(const glm::quat& worldRot);

        /// Set the local orientation of the entity.
        /**
         * @param localRot The local rotation you want the entity to have.
         */
        ENGINE_API void SetLocalOrientation(const glm::quat& localRot);

        /// Rotates around the Y axis
        /**
         * @param angle The angle in radians that we want to rotate.
         */
        ENGINE_API void RotateYaw(float angle);

        /// Rotates around the X axis
        /**
         * @param angle The angle in radians that we want to rotate.
         */
        ENGINE_API void RotatePitch(float angle);

        /// Rotates around the Z axis
        /**
         * @param angle The angle in radians that we want to rotate.
         */
        ENGINE_API void RotateRoll(float angle);

        /// Rotates around an axis given in world space.
        /**
         * @param angle The angle in radians that we want to rotate.
         * @param axis World space axis to rotate around.
         */
        ENGINE_API void RotateAroundWorldAxis(float angle, const glm::vec3& axis);
        
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
        
        /// Quaternion describing rotation and angle of entity.
        /**
         * Default: 0 radians around y axis.
         */
        glm::quat rotation = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));

        /// Get the entity's UID
        /**
         * @return The entity's UID
         */
        ENGINE_API unsigned int GetUniqueIdentifier() const;
           
        /// Set the entity's UID
        /**
         * @param UID the entity's unique identifier to be set
         */
        ENGINE_API void SetUniqueIdentifier(unsigned int UID);

        /// Whether the entity is active.
        bool enabled = true;

        /// Whether the entity is static.
        bool isStatic = false;

        
    private:
        template<typename T> void Save(Json::Value& node, const std::string& name) const;
        template<typename T> void Load(const Json::Value& node, const std::string& name);
        ENGINE_API Component::SuperComponent* AddComponent(std::type_index componentType);
        ENGINE_API Component::SuperComponent* GetComponent(std::type_index componentType) const;
        ENGINE_API void KillComponent(std::type_index componentType);
        ENGINE_API void LoadComponent(std::type_index componentType, const Json::Value& node);
        void KillHelper();
        
        World* world;
        Entity* parent = nullptr;
        std::vector<Entity*> children;
        bool scene = false;
        std::string sceneName;

        std::map<std::type_index, Component::SuperComponent*> components;
        
        bool killed = false;
        unsigned int uniqueIdentifier = 0;
};

template<typename T> T* Entity::AddComponent() {
    std::type_index componentType = std::type_index(typeid(T*));
    return static_cast<T*>(AddComponent(componentType));
}

template<typename T> T* Entity::GetComponent() const {
    return static_cast<T*>(GetComponent(std::type_index(typeid(T*))));
}

template <typename T> void Entity::KillComponent() {
    KillComponent(typeid(T*));
}

template<typename T> void Entity::Save(Json::Value& node, const std::string& name) const {
    Component::SuperComponent* component = GetComponent(std::type_index(typeid(T*)));
    if (component != nullptr)
        node[name] = component->Save();
}

template<typename T> void Entity::Load(const Json::Value& node, const std::string& name) {
    Json::Value componentNode = node[name];
    if (!componentNode.isNull()) {
        std::type_index componentType = std::type_index(typeid(T*));
        LoadComponent(componentType, componentNode);
    }
}
