#include "ParticleSystem.hpp"
#include "../Util/Json.hpp"

Component::ParticleSystemComponent::ParticleSystemComponent()
{
}

ENGINE_API Json::Value Component::ParticleSystemComponent::Save() const
{
    Json::Value component;

    component["textureIndex"] = particleType.textureIndex;
    component["velocity"] = Json::SaveVec3(particleType.velocity);
    component["color"] = Json::SaveVec3(particleType.color);

    return component;
}
