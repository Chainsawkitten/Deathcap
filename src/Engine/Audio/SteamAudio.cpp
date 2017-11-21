#include "SteamAudio.hpp"
#include <assert.h>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

SteamAudio::SteamAudio() {
    playerPos = IPLVector3{ 0,0,0 };
    playerDir = IPLVector3{ 1,0,0 };
    playerUp = IPLVector3{ 0,1,0 };
}

SteamAudio::~SteamAudio() {
    if (renderers)
        delete renderers;
}

void SteamAudio::Process(std::vector<SteamAudio::SoundSourceInfo>& inputs, IPLAudioBuffer& output) {

    std::vector<IPLAudioBuffer> audioBuffers;
    audioBuffers.resize(inputs.size());
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        SteamAudio::SoundSourceInfo& input = inputs[i];
        renderers->Process(input.buffer, playerPos, playerDir, playerUp, input.position, input.radius, audioBuffers[i]);
    }

    assert(!audioBuffers.empty());
    output.format = audioBuffers[0].format;
    iplMixAudioBuffers(audioBuffers.size(), audioBuffers.data(), output);

    for (std::size_t i = 0; i < inputs.size(); ++i) {
        delete[] audioBuffers[i].interleavedBuffer;
    }
}

void SteamAudio::SetPlayer(IPLVector3 pos, IPLVector3 dir, IPLVector3 up) {
    playerPos = pos;
    playerDir = dir;
    playerUp = up;
}

void SteamAudio::CreateRenderers(IPLhandle environment) {
    if (renderers)
        delete renderers;
    renderers = new SteamAudioRenderers(environment);
}
