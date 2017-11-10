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
#include <Video/ParticleRenderer.hpp>
#include "../Util/Json.hpp"
#include <Utility/Log.hpp>

using namespace Video;

ParticleManager::ParticleManager() {
    randomEngine.seed(randomDevice());
    
    textureAtlas = Managers().resourceManager->CreateTexture2D(PARTICLEATLAS_PNG, PARTICLEATLAS_PNG_LENGTH);
    
    particleRenderer = new ParticleRenderer(maxParticleCount);
}

ParticleManager::~ParticleManager() {
    delete particleRenderer;
    
    Managers().resourceManager->FreeTexture2D(textureAtlas);    
}

unsigned int ParticleManager::GetMaxParticleCount() const {
    return maxParticleCount;
}

void ParticleManager::Update(World& world, float time, bool preview) {
    // Remove old particles.
    for (unsigned int i = 0; i < world.GetParticleCount(); ++i) {
        world.GetParticles()[i].life += time;
        if (world.GetParticles()[i].life >= world.GetParticles()[i].lifetime) {
            world.GetParticles()[i--] = world.GetParticles()[world.GetParticleCount() - 1];
            world.SetParticleCount(world.GetParticleCount() - 1);
        }
    }

    for (Component::ParticleSystemComponent* comp : particleSystems.GetAll()) {
        emitterSettings[comp].worldPos = comp->entity->GetModelMatrix() * glm::vec4(comp->entity->GetWorldPosition(), 1.0);
        particleSystemRenderers[comp].Update(0.1f, emitterSettings[comp]);
        emitterSettings[comp] = comp->particleType;
    }
    
    // Spawn new particles from emitters.
    std::uniform_real_distribution<float> minusOneToOne(-1.f, 1.f);
    for (Component::ParticleEmitter* emitter : particleEmitters.GetAll()) {
        if (emitter->IsKilled() || (preview && !emitter->preview) || !emitter->entity->enabled)
            continue;
        
        emitter->timeToNext -= time;
        while (emitter->timeToNext < 0.f) {
            emitter->timeToNext += emitter->averageEmitTime + minusOneToOne(randomEngine) * emitter->emitTimeVariance;
            EmitParticle(world, emitter);
        }
    }
}

void ParticleManager::UpdateBuffer(World& world) {
    particleRenderer->SetBufferContents(world.GetParticleCount(), world.GetParticles());
}

void ParticleManager::Render(World& world, const glm::vec3& position, const glm::vec3& up, const glm::mat4& viewProjectionMatrix) {
    particleRenderer->Render(textureAtlas, textureAtlasRowNumber, position, up, viewProjectionMatrix);
}

void ParticleManager::UpdateParticleSystem(World& world, Component::ParticleSystemComponent* particleSystem) {
}

void ParticleManager::RenderParticleSystem(const glm::mat4& viewProjectionMatrix) {

    for (Component::ParticleSystemComponent* comp : particleSystems.GetAll()) {
        particleSystemRenderers[comp].Draw(textureAtlas, textureAtlasRowNumber, viewProjectionMatrix, emitterSettings[comp]);
    }
}

const Texture2D* ParticleManager::GetTextureAtlas() const {
    return textureAtlas;
}

int ParticleManager::GetTextureAtlasRows() const {
    return textureAtlasRowNumber;
}

Component::ParticleEmitter* ParticleManager::CreateParticleEmitter() {
    return particleEmitters.Create();
}

Component::ParticleSystemComponent* ParticleManager::CreateAParticleSystem(World * world) {

    return InitParticleSystem(particleSystems.Create());
}

Component::ParticleEmitter* ParticleManager::CreateParticleEmitter(const Json::Value& node) {
    Component::ParticleEmitter* particleEmitter = particleEmitters.Create();
    
    // Load values from Json node.
    particleEmitter->particleType.textureIndex = node.get("textureIndex", 0).asInt();
    particleEmitter->particleType.minVelocity = Json::LoadVec3(node["minVelocity"]);
    particleEmitter->particleType.maxVelocity = Json::LoadVec3(node["maxVelocity"]);
    particleEmitter->particleType.averageLifetime = node.get("averageLifetime", 0.f).asFloat();
    particleEmitter->particleType.lifetimeVariance = node.get("lifetimeVariance", 0.f).asFloat();
    particleEmitter->particleType.averageSize = Json::LoadVec2(node["averageSize"]);
    particleEmitter->particleType.sizeVariance = Json::LoadVec2(node["sizeVariance"]);
    particleEmitter->particleType.uniformScaling = node.get("uniformScaling", false).asBool();
    particleEmitter->particleType.startAlpha = node.get("startAlpha", 0.f).asFloat();
    particleEmitter->particleType.midAlpha = node.get("midAlpha", 1.f).asFloat();
    particleEmitter->particleType.endAlpha = node.get("endAlpha", 0.f).asFloat();
    particleEmitter->particleType.color = Json::LoadVec3(node["color"]);
    particleEmitter->size = Json::LoadVec3(node["size"]);
    particleEmitter->averageEmitTime = node.get("averageEmitTime", 0.03).asFloat();
    particleEmitter->emitTimeVariance = node.get("emitTimeVariance", 0.03).asFloat();
    particleEmitter->emitterType = static_cast<Component::ParticleEmitter::EmitterType>(node.get("emitterType", 0).asInt());
    
    return particleEmitter;
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

const std::vector<Component::ParticleEmitter*>& ParticleManager::GetParticleEmitters() const {
    return particleEmitters.GetAll();
}

void ParticleManager::ClearKilledComponents() {
    particleEmitters.ClearKilled();
    particleSystems.ClearKilled();
}

Component::ParticleSystemComponent* ParticleManager::InitParticleSystem(Component::ParticleSystemComponent* component) {
    ParticleSystemRenderer PS_Renderer;
    ParticleSystemRenderer::EmitterSettings setting;
    emitterSettings[component] = setting;
    particleSystemRenderers[component] = PS_Renderer;
    particleSystemRenderers[component].Init();
    particleSystemRenderers[component].CreateStorageBuffers();

    return component;
}

void ParticleManager::EmitParticle(World& world, Component::ParticleEmitter* emitter) {
    glm::vec3 position(emitter->entity->GetWorldPosition());
    if (emitter->emitterType == Component::ParticleEmitter::CUBOID) {
        std::uniform_real_distribution<float> randomSpread(-0.5f, 0.5f);
        glm::vec3 random(randomSpread(randomEngine), randomSpread(randomEngine), randomSpread(randomEngine));
        position += random * emitter->size;
    }
    EmitParticle(world, position, emitter);
}

void ParticleManager::EmitParticle(World& world, const glm::vec3& position, Component::ParticleEmitter* emitter) {
    if (world.GetParticleCount() < maxParticleCount) {
        Video::ParticleRenderer::Particle particle;
        std::uniform_real_distribution<float> zeroToOne(0.f, 1.f);
        std::uniform_real_distribution<float> minusOneToOne(-1.f, 1.f);
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = emitter->particleType.averageLifetime + minusOneToOne(randomEngine) * emitter->particleType.lifetimeVariance;
        particle.textureIndex = static_cast<float>(emitter->particleType.textureIndex);
        particle.alpha[0] = emitter->particleType.startAlpha;
        particle.alpha[1] = emitter->particleType.midAlpha;
        particle.alpha[2] = emitter->particleType.endAlpha;
        particle.color = emitter->particleType.color;
        
        if (emitter->particleType.uniformScaling) {
            particle.size = emitter->particleType.averageSize + minusOneToOne(randomEngine) * emitter->particleType.sizeVariance;
        } else {
            particle.size.x = emitter->particleType.averageSize.x + minusOneToOne(randomEngine) * emitter->particleType.sizeVariance.x;
            particle.size.y = emitter->particleType.averageSize.y + minusOneToOne(randomEngine) * emitter->particleType.sizeVariance.y;
        }
        
        particle.velocity.x = emitter->particleType.minVelocity.x + zeroToOne(randomEngine) * (emitter->particleType.maxVelocity.x - emitter->particleType.minVelocity.x);
        particle.velocity.y = emitter->particleType.minVelocity.y + zeroToOne(randomEngine) * (emitter->particleType.maxVelocity.y - emitter->particleType.minVelocity.y);
        particle.velocity.z = emitter->particleType.minVelocity.z + zeroToOne(randomEngine) * (emitter->particleType.maxVelocity.z - emitter->particleType.minVelocity.z);
        
        world.GetParticles()[world.GetParticleCount()] = particle;
        world.SetParticleCount(world.GetParticleCount() + 1);
    } else {
        Log() << "ParticleManager:EmitParticle: Warning: Exceeding max number of particles.\n";
    }
}
