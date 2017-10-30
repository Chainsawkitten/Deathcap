#pragma once

class SuperTrigger {
    public:
        SuperTrigger();
        virtual ~SuperTrigger();

        virtual void Process() = 0;
};
