#include "Resources.hpp"

#include <json/value.h>
#include <Engine/Texture/TextureAsset.hpp>
#include <Engine/Geometry/Model.hpp>
#include <Engine/Audio/SoundBuffer.hpp>
#include <Engine/Hymn.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ResourceManager.hpp>
#include <Engine/Util/FileSystem.hpp>

using namespace std;

ResourceList::ResourceList() {
    activeScene = "";
}

ResourceList& ResourceList::GetInstance() {
    static ResourceList resourceList;
    
    return resourceList;
}

void ResourceList::Save() const {
    Json::Value root;
    
    root["activeScene"] = activeScene;
    
    // Save resources.
    Json::Value resourcesNode;
    for (const Resource& resource : resources) {
        Json::Value resourceNode;
        resourceNode["type"] = resource.type;
        
        switch (resource.type) {
        case Resource::TEXTURE:
            resourceNode["texture"] = resource.texture->name;
            resource.texture->Save();
            break;
        case Resource::MODEL:
            resourceNode["model"] = resource.model->name;
            resource.model->Save();
            break;
        case Resource::SOUND:
            resourceNode["sound"] = resource.sound->name;
            resource.sound->Save();
            break;
        case Resource::SCENE:
            resourceNode["scene"] = resource.scene;
            break;
        }
        
        resourcesNode.append(resourceNode);
    }
    root["resources"] = resourcesNode;
    
    // Save to file.
    ofstream file(Hymn().GetPath() + FileSystem::DELIMITER + "Resources.json");
    file << root;
    file.close();
}

void ResourceList::Load() {
    // Load Json document from file.
    Json::Value root;
    ifstream file(Hymn().GetPath() + FileSystem::DELIMITER + "Resources.json");
    file >> root;
    file.close();
    
    activeScene = root["activeScene"].asString();
    
    // Load resources.
    Json::Value resourcesNode = root["resources"];
    
    for (unsigned int i = 0; i < resourcesNode.size(); ++i) {
        Json::Value resourceNode = resourcesNode[i];
        Resource resource;
        resource.type = static_cast<Resource::Type>(resourceNode["type"].asInt());
        
        switch (resource.type) {
        case Resource::TEXTURE:
            resource.texture = Managers().resourceManager->CreateTextureAsset(resourceNode["texture"].asString());
            break;
        case Resource::MODEL:
            resource.model = Managers().resourceManager->CreateModel(resourceNode["model"].asString());
            break;
        case Resource::SOUND:
            resource.sound = Managers().resourceManager->CreateSound(resourceNode["sound"].asString());
            break;
        case Resource::SCENE:
            resource.scene = resourceNode["scene"].asString();
            break;
        }
        
        resources.push_back(resource);
    }
    
    /*textureNumber = textures.size();
    modelNumber = models.size();
    soundNumber = sounds.size();*/
}

void ResourceList::Clear() {
    // Clear resources.
    for (const Resource& resource : resources) {
        switch (resource.type) {
        case Resource::Type::MODEL:
            Managers().resourceManager->FreeModel(resource.model);
            break;
        case Resource::Type::TEXTURE:
            Managers().resourceManager->FreeTextureAsset(resource.texture);
            break;
        case Resource::Type::SOUND:
            Managers().resourceManager->FreeSound(resource.sound);
            break;
        default:
            break;
        }
    }
    resources.clear();
    
    modelNumber = 0U;
    textureNumber = 0U;
    soundNumber = 0U;
}

ResourceList& Resources() {
    return ResourceList::GetInstance();
}
