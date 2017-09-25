#include "Material.hpp"

#include "../Manager/Managers.hpp"
#include "../Manager/ResourceManager.hpp"
#include "../Texture/TextureAsset.hpp"
#include "../Hymn.hpp"

using namespace Component;

Material::Material(Entity* entity) : SuperComponent(entity) {
    diffuse = Hymn().defaultDiffuse;
    normal = Hymn().defaultNormal;
    specular = Hymn().defaultSpecular;
    glow = Hymn().defaultGlow;
}

Material::~Material() {
    if (diffuse != nullptr && diffuse != Hymn().defaultDiffuse)
        Managers().resourceManager->FreeTextureAsset(diffuse);
    
    if (normal != nullptr && normal != Hymn().defaultNormal)
        Managers().resourceManager->FreeTextureAsset(normal);
    
    if (specular != nullptr && specular != Hymn().defaultSpecular)
        Managers().resourceManager->FreeTextureAsset(diffuse);
    
    if (glow != nullptr && glow != Hymn().defaultGlow)
        Managers().resourceManager->FreeTextureAsset(glow);
}

Json::Value Material::Save() const {
    Json::Value component;
    
    if (diffuse != nullptr)
        component["diffuse"] = diffuse->name;
    
    if (normal != nullptr)
        component["normal"] = normal->name;
    
    if (specular != nullptr)
        component["specular"] = specular->name;
    
    if (glow != nullptr)
        component["glow"] = glow->name;
    
    return component;
}

void Material::Load(const Json::Value& node) {
    LoadTexture(diffuse, node.get("diffuse", "").asString());
    LoadTexture(normal, node.get("normal", "").asString());
    LoadTexture(specular, node.get("specular", "").asString());
    LoadTexture(glow, node.get("glow", "").asString());
}

void Material::LoadTexture(TextureAsset*& texture, const std::string& name) {
    if (!name.empty())
        texture = Managers().resourceManager->CreateTextureAsset(name);
}
