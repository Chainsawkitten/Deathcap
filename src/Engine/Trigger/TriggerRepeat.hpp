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
    int m_targetID = 0;
    bool isGettingRemoved = false;
};

namespace Physics {
    class Trigger;
}

class TriggerRepeat : public SuperTrigger {
    friend class ::TriggerManager;

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

    ENGINE_API std::string GetName();
    ENGINE_API void SetName(std::string value);

    ENGINE_API std::vector<std::string> GetTargetFunction();
    ENGINE_API void SetTargetFunction(std::string value);

    ENGINE_API bool GetStartActive();
    ENGINE_API void SetStartActive(bool value);

    ENGINE_API float GetDelay();
    ENGINE_API void SetDelay(float value);

    ENGINE_API float GetCooldown();
    ENGINE_API void SetCooldown(float value);

    ENGINE_API float GetTriggerCharges();
    ENGINE_API void SetTriggerCharges(int value);

    ENGINE_API std::vector<Entity*> *GetTargetEntity();

    ENGINE_API Entity* GetCollidedEntity();
    ENGINE_API void SetCollidedEntity(Entity* value);

    ENGINE_API std::vector<EventStruct>* GetEventVector();

private:
    void HandleTriggerEvent();

    std::string name = "New Trigger";
    std::vector<std::string> targetFunction;
    bool startActive = false;
    float delay = 0;
    float cooldown = 0;
    int triggerCharges = 0;
    std::vector<Entity*> targetEntity;
    Entity* collidedEntity = nullptr;
    Utility::LockBox<Physics::Trigger> triggerVolume;
    bool triggered = false;
    std::vector<EventStruct> eventVector;
};