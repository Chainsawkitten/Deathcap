#pragma once

#include "SuperComponent.hpp"

class SuperTrigger;
class TriggerManager;

namespace Json {
    class Value;
}

namespace Component {
    class Trigger : public SuperComponent {
        friend class ::TriggerManager;

    public:
        enum TriggerTypes {
            ONCE = 0,
            REPEAT,
            LOOK_AT,
            PROXIMITY,
            NUMBER_OF_TYPES
        };

        Trigger();

        ~Trigger() override;

        ENGINE_API Json::Value Save() const override;

        ENGINE_API SuperTrigger* GetTrigger();
        ENGINE_API void SetTrigger(SuperTrigger* trigger);

        TriggerTypes triggerType = TriggerTypes::ONCE;

    private:
        SuperTrigger* m_trigger = nullptr;
    };
}