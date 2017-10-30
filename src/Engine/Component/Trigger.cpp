#include "Trigger.hpp"
#include "../Trigger/SuperTrigger.hpp"

using namespace Component;

Trigger::Trigger() {
    // ADD CONSTRUCTOR
}

Trigger::~Trigger() {
    // ADD DECONSTRUCT
}

Json::Value Trigger::Save() const {
    // ADD SAVE
    Json::Value component;

<<<<<<< HEAD
=======
    component["x"] = 0;

>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
    return component;
}

SuperTrigger* Trigger::GetTrigger() {
    return m_trigger;
}

void Trigger::SetTrigger(SuperTrigger* trigger) {
<<<<<<< HEAD

    if (m_trigger != nullptr)
        delete m_trigger;

=======
    delete m_trigger;
>>>>>>> 27b038f5d15d1d78d7eea09191ce72cae328e2b5
    m_trigger = trigger;
}
