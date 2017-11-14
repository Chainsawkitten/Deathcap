#pragma once

#include <string>
#include "../linking.hpp"
#include "SuperTrigger.hpp"

class Entity;

/// %Trigger that can be executed once before going inactive.
class TriggerOnce : public SuperTrigger {
    public:
        /// PLEASE DO NOT USE THIS.
        /// THIS CLASS IS STILL UNDER CONSTRUCTION.
        /// PLEASE DO NOT USE THIS.

        ENGINE_API TriggerOnce();
        ENGINE_API ~TriggerOnce();

        /// Setup the trigger to listen for `enter` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnEnter();

        /// Setup the trigger to listen for `retain` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnLeave();

        /// Setup the trigger to listen for `leave` events on the trigger
        /// volume, forgetting any previously set listener
        ENGINE_API void OnRemain();

        /// Get the name of the trigger.
        /**
         * @return The name of the trigger.
         */
        ENGINE_API const std::string& GetName() const;

        /// Set the name of the trigger.
        /**
         * @param value The name of the trigger.
         */
        ENGINE_API void SetName(const std::string& value);

        /// Get name of target function.
        /**
         * @return Name of target function.
         */
        ENGINE_API const std::string& GetTargetFunction() const;

        /// Set name of target function.
        /**
         * @param value Name of target function.
         */
        ENGINE_API void SetTargetFunction(const std::string& value);

        /// If trigger is starting active or not.
        /**
         * @return If trigger is starting active or not.
         */
        ENGINE_API bool GetStartActive() const;

        /// Set if trigger is starting active or not.
        /**
         * @param value If trigger is starting active or not.
         */
        ENGINE_API void SetStartActive(bool value);

        /// Get the delay before the trigger gets active.
        /**
         * @return The delay before the trigger gets active.
         */
        ENGINE_API float GetDelay() const;

        /// Set the delay before the trigger gets active.
        /**
         * @param value The delay before the trigger gets active.
         */
        ENGINE_API void SetDelay(float value);

        /// Pointer to target entity.
        /**
         * @return Pointer to target entity.
         */
        ENGINE_API Entity* GetTargetEntity();

        /// Set pointer to target entity.
        /**
         * @param value Pointer to target entity.
         */
        ENGINE_API void SetTargetEntity(Entity* value);

        /// Pointer to collided entity.
        /**
         * @return Pointer to collided entity.
         */
        ENGINE_API Entity* GetCollidedEntity();

        /// Set pointer to collided entity.
        /**
         * @param value Pointer to collided entity.
         */
        ENGINE_API void SetCollidedEntity(Entity* value);

    private:
        std::string name = "DEBUG";
        std::string targetFunction = "DEBUG";
        bool startActive;
        float delay = 0;
        Entity* targetEntity = nullptr;
        Entity* collidedEntity = nullptr;
};
