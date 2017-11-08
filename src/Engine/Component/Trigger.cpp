#include "Trigger.hpp"
#include "../Trigger/SuperTrigger.hpp"

using namespace Component;

Trigger::Trigger() {
    // ADD CONSTRUCTOR
}

Trigger::~Trigger() {

    if(m_trigger != nullptr)
        delete m_trigger;
}

Json::Value Trigger::Save() const {
    Json::Value component;

    component = m_trigger->Save();

    return component;
}

SuperTrigger* Trigger::GetTrigger() {
    return m_trigger;
}

void Trigger::SetTrigger(SuperTrigger* trigger) {
    delete m_trigger;
    m_trigger = trigger;
}