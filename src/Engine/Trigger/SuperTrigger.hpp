#pragma once

namespace Json {
    class Value;
}

class SuperTrigger {
    public:
        SuperTrigger();
        virtual ~SuperTrigger();

        virtual void Process() = 0;
        virtual void Update() = 0;
        virtual Json::Value Save() = 0;
        virtual void InitTriggerUID() = 0;
};
