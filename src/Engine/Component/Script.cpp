#include "Script.hpp"

#include <string>
#include <angelscript.h>
#include "../Manager/Managers.hpp"
#include "../Manager/ResourceManager.hpp"
#include "../Manager/ScriptManager.hpp"
#include "../Entity/Entity.hpp"
#include "../Script/ScriptFile.hpp"

using namespace Component;

Script::Script() {
    
}

Script::~Script() {
    if (instance != nullptr)
        instance->Release();
    
    if (scriptFile != nullptr)
        Managers().resourceManager->FreeScriptFile(scriptFile);
}

Json::Value Script::Save() const {
    Json::Value component;
    if (scriptFile != nullptr)
        component["scriptName"] = scriptFile->name;
    
    for (auto &name_map : propertyMap) {

        std::string name = name_map.first.c_str();

        for (auto &typeId_value : name_map.second) {

            int typeId = typeId_value.first;
            void* varPointer = typeId_value.second;
            if (typeId == asTYPEID_INT32)
            {
                component["propertyMap"][name][typeId] = *(int*)varPointer;
            }
            else if (typeId == asTYPEID_FLOAT)
            {
                component["propertyMap"][name][typeId] = *(float*)varPointer;
            }
            else if (typeId == instance->GetEngine()->GetTypeIdByDecl("string"))
            {
                std::string *str = (std::string*)varPointer;
                component["propertyMap"][name][typeId] = str;
            }
        }
    }

    return component;
}
