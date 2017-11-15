#include "SteamAudioIndirectRenderer.hpp"

SteamAudioIndirectRenderer::SteamAudioIndirectRenderer() {
    environment = nullptr;
}

SteamAudioIndirectRenderer::SteamAudioIndirectRenderer(IPLhandle environment) {
    this->environment = environment;
}

SteamAudioIndirectRenderer::~SteamAudioIndirectRenderer() {

}
