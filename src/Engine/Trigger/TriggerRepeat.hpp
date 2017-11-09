#pragma once

#include <memory>
#include <string>
#include <Utility/LockBox.hpp>
#include <vector>
#include "../linking.hpp"
#include "SuperTrigger.hpp"


class Entity;
class TriggerManager;

struct EventStruct {
    int m_eventID = 0;
    int m_shapeID = 0;
    int m_targetID = 0;
    int m_scriptID = 0;
    bool check[4] = { false }; /// Simple check, should probably be replaced soon.
};

namespace Physics {
    class Trigger;
}

class TriggerRepeat : public SuperTrigger {
    friend class ::TriggerManager;
    /// %Trigger that can be executed multiple times.
    public:
        ENGINE_API TriggerRepeat();
        ENGINE_API ~TriggerRepeat();

        /// Setup the trigger to listen for `enter` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnEnter();

        /// Setup the trigger to listen for `retain` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnRetain();

        /// Setup the trigger to listen for `leave` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnLeave();

        /// Get the name of the trigger.
        /**
        * @return The name of the trigger.
        */
        ENGINE_API std::string GetName();

        /// Set the name of the trigger.
        /**
        * @param value The name of the trigger.
        */
        ENGINE_API void SetName(std::string value);

        /// Vector containing name of target functions.
        /**
        * @return Pointer to the vector containing name of target functions.
        */
        ENGINE_API std::vector<std::string> *GetTargetFunction();

        /// Get if the trigger starts active or not.
        /**
        * @return If the trigger starts active or not.
        */
        ENGINE_API bool GetStartActive();

        /// Get if the trigger starts active or not.
        /**
        * @param value If the trigger starts active or not.
        */
        ENGINE_API void SetStartActive(bool value);

        /// Get the delay before the trigger gets active.
        /**
        * @return The delay before the trigger gets active.
        */
        ENGINE_API float GetDelay();

        /// Set the delay before the trigger gets active.
        /**
        * @param value The delay before the trigger gets active.
        */
        ENGINE_API void SetDelay(float value);

        /// Get the cooldown before the trigger can get activated again.
        /**
        * @return The cooldown before the trigger can get activated again.
        */
        ENGINE_API float GetCooldown();

        /// Set the cooldown before the trigger can get activated again.
        /**
        * @param value The cooldown before the trigger can get activated again.
        */
        ENGINE_API void SetCooldown(float value);

        /// Get the amount of times a trigger can be activated before it is no longer active.
        /**
        * @return The amount of times a trigger can be activated before it is no longer active.
        */
        ENGINE_API float GetTriggerCharges();

        /// Set the amount of times a trigger can be activated before it is no longer active.
        /**
        * @param value The amount of times a trigger can be activated before it is no longer active.
        */
        ENGINE_API void SetTriggerCharges(int value);

        /// Vector containing target entities.
        /**
        * @return Pointer to the vector containing target entities.
        */
        ENGINE_API std::vector<Entity*> *GetTargetEntity();

        /// Vector containing collided entities.
        /**
        * @return Pointer to the vector containing collided entities.
        */
        ENGINE_API std::vector<Entity*> *GetCollidedEntity();

        /// Vector containing the event data.
        /**
        * @return Pointer to the vector containing event data.
        */
        ENGINE_API std::vector<EventStruct>* GetEventVector();

        /// Get the entity this component belongs to.
        /**
        * @return The entity this component belongs to.
        */
        ENGINE_API Entity* GetOwningEntity();

        /// Set the entity this component belongs to.
        /**
        * @param value The entity this component belongs to.
        */
        ENGINE_API void SetOwningEntity(Entity* value);
                
        ENGINE_API void Process() override;
        ENGINE_API void Update() override;
        ENGINE_API Json::Value Save() override;
        ENGINE_API void InitTriggerUID() override;

    private:
        void HandleTriggerEvent();

        std::string name = "New Trigger";
        bool startActive = false;
        float delay = 0;
        float cooldown = 0;
        int triggerCharges = 0;
        std::vector<std::string> targetFunction;
        std::vector<Entity*> targetEntity;
        std::vector<Entity*> collidedEntity;
        std::vector<EventStruct> eventVector;
        Utility::LockBox<Physics::Trigger> triggerVolume;
        bool triggered = false;
        Entity* owningEntity = nullptr;

        int targetEntityUID = 0;
        int collidedEntityUID = 0;
        int owningEntityUID = 0;
};
