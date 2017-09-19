#include "Hymn.hpp"

#include "Util/FileSystem.hpp"
#include "Manager/Managers.hpp"
#include "Manager/RenderManager.hpp"
#include "Manager/PhysicsManager.hpp"
#include "Manager/ParticleManager.hpp"
#include "Manager/ScriptManager.hpp"
#include "Manager/SoundManager.hpp"
#include "Manager/DebugDrawingManager.hpp"
#include "DefaultDiffuse.png.hpp"
#include "DefaultNormal.png.hpp"
#include "DefaultSpecular.png.hpp"
#include "DefaultGlow.png.hpp"
#include "Geometry/Model.hpp"
#include <Video/Texture/Texture2D.hpp>
#include "Texture/TextureAsset.hpp"
#include "Audio/SoundBuffer.hpp"
#include "Input/Input.hpp"
#include "Script/ScriptFile.hpp"
#include "Util/Json.hpp"
#include <fstream>
#include "Util/Profiling.hpp"

#include "Entity/Entity.hpp"
#include "Component/Animation.hpp"

using namespace std;

ActiveHymn::ActiveHymn() {
    defaultDiffuse = new TextureAsset();
    defaultDiffuse->GetTexture()->Load(DEFAULTDIFFUSE_PNG, DEFAULTDIFFUSE_PNG_LENGTH, true);
    defaultNormal = new TextureAsset();
    defaultNormal->GetTexture()->Load(DEFAULTNORMAL_PNG, DEFAULTNORMAL_PNG_LENGTH, false);
    defaultSpecular = new TextureAsset();
    defaultSpecular->GetTexture()->Load(DEFAULTSPECULAR_PNG, DEFAULTSPECULAR_PNG_LENGTH, false);
    defaultGlow = new TextureAsset();
    defaultGlow->GetTexture()->Load(DEFAULTGLOW_PNG, DEFAULTGLOW_PNG_LENGTH, false);
    
    Clear();
}

ActiveHymn& ActiveHymn::GetInstance() {
    static ActiveHymn ActiveHymn;
    
    return ActiveHymn;
}

void ActiveHymn::Clear() {
    path = "";
    world.Clear();
    
    entityNumber = 1U;
    
    scenes.clear();
    
    for (Geometry::Model* model : models) {
        delete model;
    }
    models.clear();
    modelNumber = 0U;
    
    for (TextureAsset* texture : textures) {
        delete texture;
    }
    textures.clear();
    textureNumber = 0U;
    
    for (Audio::SoundBuffer* sound : sounds) {
        delete sound;
    }
    sounds.clear();
    soundNumber = 0U;
    
    for (ScriptFile* script : scripts) {
        delete script;
    }
    scripts.clear();
    scriptNumber = 0U;
    
    filterSettings.color = false;
    filterSettings.fog = false;
    filterSettings.fogDensity = 0.001f;
    filterSettings.fxaa = true;
    filterSettings.glow = true;
    filterSettings.glowBlurAmount = 1;
}

const string& ActiveHymn::GetPath() const {
    return path;
}

void ActiveHymn::SetPath(const string& path) {
    this->path = path;
    FileSystem::CreateDirectory(path.c_str());
    FileSystem::CreateDirectory((path + FileSystem::DELIMITER + "Models").c_str());
    FileSystem::CreateDirectory((path + FileSystem::DELIMITER + "Scenes").c_str());
    FileSystem::CreateDirectory((path + FileSystem::DELIMITER + "Scripts").c_str());
    FileSystem::CreateDirectory((path + FileSystem::DELIMITER + "Sounds").c_str());
    FileSystem::CreateDirectory((path + FileSystem::DELIMITER + "Textures").c_str());
}



void ActiveHymn::Save() const {
    
    // Save to file.
    ofstream file(path + FileSystem::DELIMITER + "Hymn.json");
    file << ToJson();
    file.close();
}

Json::Value ActiveHymn::ToJson() const {

    Json::Value root;

    // Save textures.
    Json::Value texturesNode;
    for (TextureAsset* texture : textures) {
        texturesNode.append(texture->Save());
    }
    root["textures"] = texturesNode;

    // Save models.
    Json::Value modelsNode;
    for (Geometry::Model* model : models) {
        modelsNode.append(model->Save());
    }
    root["models"] = modelsNode;

    // Save scripts.
    Json::Value scriptNode;
    for (ScriptFile* script : scripts) {
        scriptNode.append(script->Save());
    }
    root["scripts"] = scriptNode;

    // Save sounds.
    Json::Value soundsNode;
    for (Audio::SoundBuffer* sound : sounds) {
        soundsNode.append(sound->Save());
    }
    root["sounds"] = soundsNode;

    // Save scenes.
    Json::Value scenesNode;
    for (const string& scene : scenes) {
        scenesNode.append(scene);
    }
    root["scenes"] = scenesNode;
    root["activeScene"] = activeScene;

    Json::Value inputNode;
    inputNode.append(Input::GetInstance().Save());
    root["input"] = inputNode;

    // Filter settings.
    Json::Value filtersNode;
    filtersNode["color"] = filterSettings.color;
    filtersNode["colorColor"] = Json::SaveVec3(filterSettings.colorColor);
    filtersNode["fog"] = filterSettings.fog;
    filtersNode["fogDensity"] = filterSettings.fogDensity;
    filtersNode["fogColor"] = Json::SaveVec3(filterSettings.fogColor);
    filtersNode["fxaa"] = filterSettings.fxaa;
    filtersNode["glow"] = filterSettings.glow;
    filtersNode["glowBlurAmount"] = filterSettings.glowBlurAmount;
    root["filters"] = filtersNode;

    return root;

}

void ActiveHymn::FromJson(Json::Value root) {

    // Load textures.
    const Json::Value texturesNode = root["textures"];
    for (unsigned int i = 0; i < texturesNode.size(); ++i) {
        TextureAsset* texture = new TextureAsset();
        texture->Load(texturesNode[i]);
        textures.push_back(texture);
    }

    // Load models.
    const Json::Value modelsNode = root["models"];
    for (unsigned int i = 0; i < modelsNode.size(); ++i) {
        Geometry::Model* model;
        std::string type = modelsNode[i].get("type", "").asString();
        model = new Geometry::Model();
        model->Load(modelsNode[i]);
        models.push_back(model);
    }

    // Load scripts.
    const Json::Value scriptNode = root["scripts"];
    for (unsigned int i = 0; i < scriptNode.size(); ++i) {
        ScriptFile* script = new ScriptFile();
        script->Load(scriptNode[i]);
        scripts.push_back(script);
    }

    // Load sounds.
    const Json::Value soundsNode = root["sounds"];
    for (unsigned int i = 0; i < soundsNode.size(); ++i) {
        Audio::SoundBuffer* sound = new Audio::SoundBuffer();
        sound->Load(soundsNode[i]);
        sounds.push_back(sound);
    }

    // Load scenes.
    const Json::Value scenesNode = root["scenes"];
    for (unsigned int i = 0; i < scenesNode.size(); ++i) {
        scenes.push_back(scenesNode[i].asString());
    }

    activeScene = root["activeScene"].asUInt();
    Hymn().world.Load(Hymn().GetPath() + FileSystem::DELIMITER + "Scenes" + FileSystem::DELIMITER + scenes[activeScene] + ".json");

    const Json::Value inputNode = root["input"];
    Input::GetInstance().Load(inputNode[0]);

    // Load filter settings.
    Json::Value filtersNode = root["filters"];
    filterSettings.color = filtersNode["color"].asBool();
    filterSettings.colorColor = Json::LoadVec3(filtersNode["colorColor"]);
    filterSettings.fog = filtersNode["fog"].asBool();
    filterSettings.fogDensity = filtersNode["fogDensity"].asFloat();
    filterSettings.fogColor = Json::LoadVec3(filtersNode["fogColor"]);
    filterSettings.fxaa = filtersNode["fxaa"].asBool();
    filterSettings.glow = filtersNode["glow"].asBool();
    filterSettings.glowBlurAmount = filtersNode["glowBlurAmount"].asInt();

    textureNumber = textures.size();
    modelNumber = models.size();
    soundNumber = sounds.size();
    scriptNumber = scripts.size();

}

void ActiveHymn::Load(const string& path) {
    Clear();
    this->path = path;
    
    // Load Json document from file.
    Json::Value root;
    ifstream file(path + FileSystem::DELIMITER + "Hymn.json");
    file >> root;
    file.close();
    
    FromJson(root);

}

void ActiveHymn::Update(float deltaTime) {
    { PROFILE("Run scripts.");
        Managers().scriptManager->Update(world, deltaTime);
    }
    
    { PROFILE("Update physics");
        Managers().physicsManager->Update(world, deltaTime);
    }
    
    { PROFILE("Update animations");
        for (Entity* entity : world.GetEntities()) {
            Component::Animation* anim = entity->GetComponent<Component::Animation>();
            if (anim != nullptr) {
                Geometry::Model* model = anim->riggedModel;
                /// @todo Fix animations.
            }
        }
    }
    
    { PROFILE("Update particles");
        Managers().particleManager->Update(world, deltaTime);
    }
    
    { PROFILE("Update sounds");
        Managers().soundManager->Update(world);
    }
    
    { PROFILE("Update debug drawing");
        Managers().debugDrawingManager->Update(deltaTime);
    }
    
    { PROFILE("Clear killed entities/components");
        world.ClearKilled();
    }
}

void ActiveHymn::Render(Entity* camera, bool soundSources, bool particleEmitters, bool lightSources, bool cameras) {
    { PROFILE("Render world");
        Managers().renderManager->Render(world, camera);
    }
    
    if (soundSources || particleEmitters || lightSources || cameras) {
        { PROFILE("Render editor entities");
            Managers().renderManager->RenderEditorEntities(world, camera, soundSources, particleEmitters, lightSources, cameras);
        }
    }
    
    { PROFILE("Render debug entities");
        Managers().debugDrawingManager->Render(world, camera);
    }
}

ActiveHymn& Hymn() {
    return ActiveHymn::GetInstance();
}
