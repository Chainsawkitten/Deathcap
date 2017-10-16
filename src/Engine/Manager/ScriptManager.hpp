#pragma once

#include <string>
#include <vector>
#include "../Entity/ComponentContainer.hpp"
#include "../linking.hpp"

class asIScriptEngine;
class asIScriptContext;
class asITypeInfo;
class World;
class Entity;
class ScriptFile;
namespace Component {
    class RigidBody;
    class Script;
}
namespace Json {
    class Value;
}

/// Handles scripting.
class ENGINE_EXPORT ScriptManager {
    friend class Hub;
        
    public:
        /// Build a script that can later be run.
        /**
         * @param script Script to build.
         */
        void BuildScript(const ScriptFile* script);
        
        /// Build all scripts in the hymn.
        void BuildAllScripts();
        
        ///Fetches the properties from the script and fills the map.
        /**
         * @param script The script which map to update.
         */
        void FillPropertyMap(Component::Script* script);

        /// Update all script entities in the game world.
        /**
         * @param world The world to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(World& world, float deltaTime);
        
        /// Register an entity to recieve update callbacks.
        /**
         * @param entity %Entity to register.
         * @todo Fix so registered entities can be removed.
         */
        void RegisterUpdate(Entity* entity);
        
        /// Register an entity to receive an event when |object| enters |trigger|.
        /**
         * @param entity %Entity to register.
         * @param trigger Trigger body to check for.
         * @param object Object to check if it enters the trigger.
         * @param methodName The name of the method to call when triggered.
         */
        void RegisterTriggerEnter(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName);

        /// Register an entity to receive an event when |object| is intersecting |trigger|.
        /**
         * @param entity %Entity to register.
         * @param trigger Trigger body to check for.
         * @param object Object to check if it intersects the trigger.
         * @param methodName The name of the method to call when triggered.
         */
        void RegisterTriggerRetain(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName);

        /// Register an entity to receive an event when |object| leaves |trigger|.
        /**
         * @param entity %Entity to register.
         * @param trigger Trigger body to check for.
         * @param object Object to check if it leaves the trigger.
         * @param methodName The name of the method to call when triggered.
         */
        void RegisterTriggerLeave(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName);
        
        /// Register the input enum.
        void RegisterInput();
        
        /// Send a message to an entity.
        /**
         * @param recipient The entity to receive the message.
         * @param type The type of message to send.
         */
        void SendMessage(Entity* recipient, int type);

        /// Fetches an entity using its GUID.
        /**
         * @param GUID The entity to receive the message.
         * @return The entity that has the corret GUID.
         */
        Entity* GetEntity(unsigned int GUID) const;
        
        /// Create script component.
        /**
         * @return The created component.
         */
        Component::Script* CreateScript();
        
        /// Create script component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        Component::Script* CreateScript(const Json::Value& node);
        
        /// Used to get the string identifier used to check if a property is a string.
        /**
         * @return The identifier of the string declaration.
         */
        int GetStringDeclarationID();

        /// Get all script components.
        /**
         * @return All script components.
         */
        const std::vector<Component::Script*>& GetScripts() const;
        
        /// Remove all killed components.
        void ClearKilledComponents();
        
        /// The entity currently being executed.
        Entity* currentEntity;
        
    private:
        struct Message {
            Entity* recipient;
            int type;
        };
        
        struct TriggerEvent {
            Entity* scriptEntity;
            std::string methodName;
            Component::RigidBody* trigger;
            Component::RigidBody* object;
        };
        
        ScriptManager();
        ~ScriptManager();
        ScriptManager(ScriptManager const&) = delete;
        void operator=(ScriptManager const&) = delete;
        
        void CreateInstance(Component::Script* script);
        void CallMessageReceived(const Message& message);
        void CallUpdate(Entity* entity, float deltaTime);
        void CallTrigger(const TriggerEvent& triggerEvent);
        void LoadScriptFile(const char* fileName, std::string& script);
        void ExecuteCall(asIScriptContext* context);
        asITypeInfo* GetClass(const std::string& moduleName, const std::string& className);
        void HandleTrigger(TriggerEvent triggerEvent);
        
        asIScriptEngine* engine;
        
        std::vector<Entity*> updateEntities;
        std::vector<Message> messages;
        std::vector<TriggerEvent> triggerEvents;
        
        ComponentContainer<Component::Script> scripts;
};
