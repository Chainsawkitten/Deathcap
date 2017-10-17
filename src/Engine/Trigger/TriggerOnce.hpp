#pragma once
#include <string>
#include "Entity\Entity.hpp"

enum Output {
    OnEnter,
    OnLeave
};

class TriggerOnce {
public:

    TriggerOnce();

    ~TriggerOnce();

    void Execute(Output type);

private:
    std::string name;
    std::string targetFunction;
    bool startActive;
    float delay;
    Entity* targetEntity;
    Entity* collidedEntity;

private:
    void OnEnter();
    void OnLeave();
};
