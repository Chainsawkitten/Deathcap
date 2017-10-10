#include "TextureAsset.hpp"

#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include <DefaultAlbedo.png.hpp>
#include <Video/Texture/Texture2D.hpp>
#include <fstream>

using namespace Video;

TextureAsset::TextureAsset() {
    texture = new Texture2D(DEFAULTALBEDO_PNG, DEFAULTALBEDO_PNG_LENGTH, false);
}

TextureAsset::~TextureAsset() {
    delete texture;
}

void TextureAsset::Save() const {
    Json::Value texture;
    texture["srgb"] = srgb;
    
    // Save properties to meta file.
    std::string filename = Hymn().GetPath() + FileSystem::DELIMITER + "Textures" + FileSystem::DELIMITER + name + ".json";
    std::ofstream file(filename);
    file << texture;
    file.close();
}

void TextureAsset::Load(const std::string& name) {
    this->name = name;
    std::string filename = Hymn().GetPath() + FileSystem::DELIMITER + "Textures" + FileSystem::DELIMITER + name;
    
    // Get properties from meta file.
    Json::Value root;
    if(!FileSystem::FileExists(std::string(filename + ".json").c_str())) {
        Save();
    }
    std::ifstream file(filename + ".json");
    file >> root;
    file.close();
    
    srgb = root.get("srgb", false).asBool();
    
    // Load texture from disk.
    texture->Load((filename + ".png").c_str(), srgb);
}

Texture2D* TextureAsset::GetTexture() const {
    return texture;
}
