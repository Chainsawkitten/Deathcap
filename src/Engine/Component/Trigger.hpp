#pragma once
<<<<<<< HEAD
#include "../Trigger/TriggerRepeat.hpp"
#include "../Trigger/TriggerOnce.hpp"
#include "SuperComponent.hpp"

class SuperTrigger;

namespace Component {

    class Trigger : public SuperComponent {
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

        ENGINE_API  Json::Value Save() const override;

        ENGINE_API  SuperTrigger* GetTrigger();

        ENGINE_API  void SetTrigger(SuperTrigger* trigger);

        TriggerTypes triggerType = TriggerTypes::ONCE;

    private:
        SuperTrigger* m_trigger = nullptr;
=======

#include "SuperComponent.hpp"

class SuperTrigger;
class TriggerManager;

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
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
    };
}
