#include "ParticleManager.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Component/ParticleSystem.hpp"
#include "../Manager/Managers.hpp"
#include "../Manager/ResourceManager.hpp"
#include <Video/Texture/Texture2D.hpp>
#include "ParticleAtlas.png.hpp"
#include "../Util/Json.hpp"
#include <Utility/Log.hpp>

using namespace Video;

ParticleManager::ParticleManager() {
    randomEngine.seed(randomDevice());
    
    textureAtlas = Managers().resourceManager->CreateTexture2D(PARTICLEATLAS_PNG, PARTICLEATLAS_PNG_LENGTH);
}

ParticleManager::~ParticleManager() {

    Managers().resourceManager->FreeTexture2D(textureAtlas);    
}

unsigned int ParticleManager::GetMaxParticleCount() const {
    return maxParticleCount;
}

void ParticleManager::Update(World& world, float time, bool preview) {
    for (Component::ParticleSystemComponent* comp : particleSystems.GetAll()) {
        emitterSettings[comp] = comp->particleType;
        emitterSettings[comp].worldPos = comp->entity->GetWorldPosition();
        particleSystemRenderers[comp]->Update(0.1f, emitterSettings[comp]);
    }
}

void ParticleManager::RenderParticleSystem(const glm::mat4& viewProjectionMatrix) {

    for (Component::ParticleSystemComponent* comp : particleSystems.GetAll()) {
        particleSystemRenderers[comp]->Draw(textureAtlas, textureAtlasRowNumber, viewProjectionMatrix, emitterSettings[comp]);
    }
}


const Texture2D* ParticleManager::GetTextureAtlas() const {
    return textureAtlas;
}

int ParticleManager::GetTextureAtlasRows() const {
    return textureAtlasRowNumber;
}

Component::ParticleSystemComponent* ParticleManager::CreateAParticleSystem(World * world) {

    return InitParticleSystem(particleSystems.Create());
}

Component::ParticleSystemComponent* ParticleManager::CreateParticleSystem(const Json::Value& node) {

    Component::ParticleSystemComponent* particleSystem = InitParticleSystem(particleSystems.Create());
    // Load values from Json node.
    particleSystem->particleType.textureIndex = node.get("textureIndex", 0).asInt();
    particleSystem->particleType.nr_new_particles = node.get("emitAmount", 8).asInt();
    particleSystem->particleType.rate = node.get("rate", 0.3).asFloat();
    particleSystem->particleType.lifetime = node.get("lifetime", 10.0).asFloat();
    particleSystem->particleType.scale = node.get("scale", 10.0).asFloat();
    particleSystem->particleType.velocity = Json::LoadVec3(node["velocity"]);
    particleSystem->particleType.alpha_control = node.get("alphaControl", 10.0).asFloat();
    particleSystem->particleType.mass = node.get("mass", 1.0).asFloat();
    particleSystem->particleType.spread = node.get("spread", 1.0).asFloat();
    particleSystem->particleType.randomVec = Json::LoadVec3(node["randomVelocity"]);
    particleSystem->particleType.velocityMultiplier = node.get("speed", 10.0).asFloat();
    particleSystem->particleType.nr_particles = node.get("NrOfParticles", 1024).asInt();

    return particleSystem;
}

void ParticleManager::RemoveParticleRenderer(Component::ParticleSystemComponent * component) {
    delete particleSystemRenderers[component];
    particleSystemRenderers.erase(component);
}

void ParticleManager::ClearKilledComponents() {
    particleSystems.ClearKilled();
}

Component::ParticleSystemComponent* ParticleManager::InitParticleSystem(Component::ParticleSystemComponent* component) {
    ParticleSystemRenderer* PS_Renderer = new ParticleSystemRenderer();
    PS_Renderer->Init();
    PS_Renderer->CreateStorageBuffers();
    ParticleSystemRenderer::EmitterSettings setting;
    emitterSettings[component] = setting;
    particleSystemRenderers[component] = PS_Renderer;

    return component;
}
