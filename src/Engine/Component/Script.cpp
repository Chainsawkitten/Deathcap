#include "Script.hpp"

#include <angelscript.h>
#include <cstring>

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
    
    for (auto name_property : propertyMap) {

        const std::string& name = name_property.first;
        int typeId = name_property.second.typeID;
        void* varPointer = name_property.second.data;
        int size = name_property.second.size;

        for (int i = 0; i < size; i++)
            component["propertyMap"][name][std::to_string(typeId)][i] = ((unsigned char*)varPointer)[i];

    }
    return component;
}

void Script::AddToPropertyMap(std::string name, int type, int size, void* data) {

    propertyMap[name] = Property(type, size, data);

}

void Script::CopyDataFromPropertyMap(std::string name, void* target){

    std::memcpy(target, propertyMap[name].data, propertyMap[name].size);
    
}

void* Script::GetDataFromPropertyMap(std::string name){

    return propertyMap[name].data;

}

bool Script::isInPropertyMap(std::string name, int type) {

    auto it = propertyMap.find(name);

    if (it != propertyMap.end())
        if (propertyMap[name].typeID == type)
            return true;

    return false;

}

/// Clears the property map.
void Script::ClearPropertyMap() {

    for (auto pair : propertyMap)
        free(pair.second.data);

    propertyMap.clear();

}