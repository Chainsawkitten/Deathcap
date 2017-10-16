#include "Script.hpp"

#include <angelscript.h>
#include "../Manager/Managers.hpp"
#include "../Manager/ScriptManager.hpp"
#include "../Manager/ResourceManager.hpp"
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

    ClearPropertyMap();

}

Json::Value Script::Save() const {
    Json::Value component;
    if (scriptFile != nullptr)
        component["scriptName"] = scriptFile->path + scriptFile->name;
    
    for (auto &name_pair : propertyMap) {

        const std::string& name = name_pair.first;

        int typeId = name_pair.second.first;
        void* varPointer = name_pair.second.second;
        if (typeId == asTYPEID_INT32){
            component["propertyMap"][name][std::to_string(typeId)] = *(int*)varPointer;
        }
        else if (typeId == asTYPEID_FLOAT){
            component["propertyMap"][name][std::to_string(typeId)] = *(float*)varPointer;
        }
        else if (typeId == Managers().scriptManager->GetStringDeclarationID()){
            std::string *str = (std::string*)varPointer;
            component["propertyMap"][name][std::to_string(typeId)] = *str;
        }
    }

    return component;
}

/// Clears the property map.
void Script::ClearPropertyMap() {

    for (auto pair : propertyMap) {

        int typeId = pair.second.first;
        void* varPointer = pair.second.second;

        if (typeId == asTYPEID_INT32) {

            delete (int*)varPointer;

        }
        else if (typeId == asTYPEID_FLOAT) {
            delete (float*)varPointer;
        }
        else if (typeId == Managers().scriptManager->GetStringDeclarationID()) {
            delete (std::string*)varPointer;
        }

    }

    propertyMap.clear();

}