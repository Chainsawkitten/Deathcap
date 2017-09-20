#include "ResourceManager.hpp"

#include <Video/Shader/Shader.hpp>
#include <Video/Shader/ShaderProgram.hpp>
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

Video::Shader* ResourceManager::CreateShader(const char* source, int sourceLength, GLenum shaderType) {
    if (shaders.find(source) == shaders.end()) {
        shaders[source].shader = new Video::Shader(source, sourceLength, shaderType);
        shadersInverse[shaders[source].shader] = source;
        shaders[source].count = 1;
    } else {
        shaders[source].count++;
    }
    
    return shaders[source].shader;
}

void ResourceManager::FreeShader(Video::Shader* shader) {
    const char* source = shadersInverse[shader];
    
    shaders[source].count--;
    if (shaders[source].count <= 0) {
        shadersInverse.erase(shader);
        delete shader;
        shaders.erase(source);
    }
}

bool ResourceManager::ShaderProgramKey::operator<(const ShaderProgramKey& other) const {
    if (computeShader < other.computeShader) return true;
    if (computeShader > other.computeShader) return false;
    
    if (vertexShader < other.vertexShader) return true;
    if (vertexShader > other.vertexShader) return false;
    
    if (tessControlShader < other.tessControlShader) return true;
    if (tessControlShader > other.tessControlShader) return false;
    
    if (tessEvaluationShader < other.tessEvaluationShader) return true;
    if (tessEvaluationShader > other.tessEvaluationShader) return false;
    
    if (geometryShader < other.geometryShader) return true;
    if (geometryShader > other.geometryShader) return false;
    
    if (fragmentShader < other.fragmentShader) return true;
    if (fragmentShader > other.fragmentShader) return false;
    
    return false;
}

Video::ShaderProgram* ResourceManager::CreateShaderProgram(std::initializer_list<const Video::Shader*> shaders) {
    ShaderProgramKey key;
    
    for (auto shader : shaders) {
        switch (shader->GetShaderType()) {
        case GL_COMPUTE_SHADER:
            key.computeShader = shader;
            break;
        case GL_VERTEX_SHADER:
            key.vertexShader = shader;
            break;
        case GL_TESS_CONTROL_SHADER:
            key.tessControlShader = shader;
            break;
        case GL_TESS_EVALUATION_SHADER:
            key.tessEvaluationShader = shader;
            break;
        case GL_GEOMETRY_SHADER:
            key.geometryShader = shader;
            break;
        case GL_FRAGMENT_SHADER:
            key.fragmentShader = shader;
            break;
        }
    }
    
    if (shaderPrograms.find(key) == shaderPrograms.end()) {
        ShaderProgramInstance shaderProgram;
        shaderProgram.shaderProgram = new Video::ShaderProgram(shaders);
        shaderProgram.count = 1;
        shaderPrograms[key] = shaderProgram;
        shaderProgramsInverse[shaderProgram.shaderProgram] = key;
    } else {
        shaderPrograms[key].count++;
    }
    
    return shaderPrograms[key].shaderProgram;
}

void ResourceManager::FreeShaderProgram(Video::ShaderProgram* shaderProgram) {
    ShaderProgramKey key = shaderProgramsInverse[shaderProgram];
    shaderPrograms[key].count--;
    
    if (shaderPrograms[key].count <= 0) {
        shaderProgramsInverse.erase(shaderProgram);
        delete shaderProgram;
        shaderPrograms.erase(key);
    }
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

Geometry::Model* ResourceManager::CreateModel(std::string filename) {
    if (models.find(filename) == models.end()) {
        models[filename].model = new Geometry::Model(filename.c_str());
        modelsInverse[models[filename].model] = filename;
        models[filename].count = 1;
    } else {
        models[filename].count++;
    }

    return models[filename].model;
}

void ResourceManager::FreeModel(Geometry::Model* model) {
    string filename = modelsInverse[model];
    
    if (models[filename].count-- <= 1) {
        modelsInverse.erase(model);
        delete model;
        models.erase(filename);
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
