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
        
        // Save texture.
        if (resource.type == Resource::TEXTURE) {
            resourceNode["texture"] = resource.texture->name;
            resource.texture->Save();
        }
        
        // Save model.
        if (resource.type == Resource::MODEL) {
            resourceNode["model"] = resource.model->name;
            resource.model->Save();
        }
        
        // Save sound.
        if (resource.type == Resource::SOUND) {
            resourceNode["sound"] = resource.sound->name;
            resource.sound->Save();
        }
        
        // Save scenes.
        if (resource.type == Resource::SCENE) {
            resourceNode["scene"] = resource.scene;
        }
        
        resourcesNode.append(resourceNode);
    }
    root["resources"] = resourcesNode;
    
    /*// Save textures.
    Json::Value texturesNode;
    for (TextureAsset* texture : textures) {
        texturesNode.append(texture->name);
        texture->Save();
    }
    root["textures"] = texturesNode;
    
    // Save models.
    Json::Value modelsNode;
    for (Geometry::Model* model : models) {
        modelsNode.append(model->name);
    }
    root["models"] = modelsNode;
    
    // Save sounds.
    Json::Value soundsNode;
    for (Audio::SoundBuffer* sound : sounds) {
        soundsNode.append(sound->name);
    }
    root["sounds"] = soundsNode;
    
    // Save scenes.
    Json::Value scenesNode;
    for (const string& scene : scenes) {
        scenesNode.append(scene);
    }
    root["scenes"] = scenesNode;*/
    
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
    
    /// @todo Load resources.
    
    /*// Load textures.
    const Json::Value texturesNode = root["textures"];
    for (unsigned int i = 0; i < texturesNode.size(); ++i) {
        textures.push_back(Managers().resourceManager->CreateTextureAsset(texturesNode[i].asString()));
    }
    
    // Load models.
    const Json::Value modelsNode = root["models"];
    for (unsigned int i = 0; i < modelsNode.size(); ++i) {
        models.push_back(Managers().resourceManager->CreateModel(modelsNode[i].asString()));
    }
    
    // Load sounds.
    const Json::Value soundsNode = root["sounds"];
    for (unsigned int i = 0; i < soundsNode.size(); ++i) {
        sounds.push_back(Managers().resourceManager->CreateSound(soundsNode[i].asString()));
    }
    
    // Load scenes.
    const Json::Value scenesNode = root["scenes"];
    for (unsigned int i = 0; i < scenesNode.size(); ++i) {
        scenes.push_back(scenesNode[i].asString());
    }
    
    textureNumber = textures.size();
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
