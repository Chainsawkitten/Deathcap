#include "ResourceManager.hpp"

#include <Video/Geometry/Rectangle.hpp>
#include "../Geometry/Cube.hpp"
#include "../Geometry/Model.hpp"
#include <Video/Texture/Texture2D.hpp>
#include "../Audio/SoundBuffer.hpp"
#include "../Audio/VorbisFile.hpp"
#include "../Texture/TextureAsset.hpp"

using namespace std;

ResourceManager::ResourceManager() {
    
}

Video::Geometry::Rectangle* ResourceManager::CreateRectangle() {
    if (rectangleCount == 0)
        rectangle = new Video::Geometry::Rectangle();
    
    rectangleCount++;
    return rectangle;
}

void ResourceManager::FreeRectangle() {
    rectangleCount--;
    
    if (rectangleCount <= 0)
        delete rectangle;
}

Geometry::Cube* ResourceManager::CreateCube() {
    if (cubeCount++ == 0)
        cube = new Geometry::Cube();
    
    return cube;
}

void ResourceManager::FreeCube() {
    if (--cubeCount <= 0)
        delete rectangle;
}

Geometry::Model* ResourceManager::CreateModel(std::string name) {
    if (models.find(name) == models.end()) {
        Geometry::Model* model = new Geometry::Model();
        model->Load(name);
        models[name].model = model;
        modelsInverse[model] = name;
        models[name].count = 1;
    } else {
        models[name].count++;
    }

    return models[name].model;
}

void ResourceManager::FreeModel(Geometry::Model* model) {
    string name = modelsInverse[model];
    
    if (models[name].count-- <= 1) {
        modelsInverse.erase(model);
        delete model;
        models.erase(name);
    }
}

Video::Texture2D* ResourceManager::CreateTexture2D(const char* data, int dataLength, bool srgb) {
    if (textures.find(data) == textures.end()) {
        textures[data].texture = new Video::Texture2D(data, dataLength, srgb);
        texturesInverse[textures[data].texture] = data;
        textures[data].count = 1;
    } else {
        textures[data].count++;
    }
    
    return textures[data].texture;
}

void ResourceManager::FreeTexture2D(Video::Texture2D* texture) {
    const char* data = texturesInverse[texture];
    
    if (textures[data].count-- <= 1) {
        texturesInverse.erase(texture);
        delete texture;
        textures.erase(data);
    }
}

TextureAsset* ResourceManager::CreateTextureAsset(std::string name) {
    if (textureAssets.find(name) == textureAssets.end()) {
        TextureAsset* textureAsset = new TextureAsset();
        textureAsset->Load(name);
        textureAssets[name].textureAsset = textureAsset;
        textureAssetsInverse[textureAsset] = name;
        textureAssets[name].count = 1;
    } else {
        textureAssets[name].count++;
    }
    
    return textureAssets[name].textureAsset;
}

void ResourceManager::FreeTextureAsset(TextureAsset* textureAsset) {
    std::string name = textureAssetsInverse[textureAsset];
    
    if (textureAssets[name].count-- <= 1) {
        textureAssetsInverse.erase(textureAsset);
        delete textureAsset;
        textureAssets.erase(name);
    }
}

Audio::SoundBuffer* ResourceManager::CreateSound(string filename) {
    if (sounds.find(filename) == sounds.end()) {
        Audio::SoundFile* soundFile = new Audio::VorbisFile(filename.c_str());
        sounds[filename].soundBuffer = new Audio::SoundBuffer(soundFile);
        delete soundFile;
        soundsInverse[sounds[filename].soundBuffer] = filename;
        sounds[filename].count = 1;
    } else {
        sounds[filename].count++;
    }
    
    return sounds[filename].soundBuffer;
}

void ResourceManager::FreeSound(Audio::SoundBuffer* soundBuffer) {
    string filename = soundsInverse[soundBuffer];
    
    if (sounds[filename].count-- <= 1) {
        soundsInverse.erase(soundBuffer);
        delete soundBuffer;
        sounds.erase(filename);
    }
}
