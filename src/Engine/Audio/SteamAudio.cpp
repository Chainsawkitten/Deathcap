#include "SteamAudio.hpp"

SteamAudio::SteamAudio() {
    playerPos = IPLVector3{ 0,0,0 };
    playerDir = IPLVector3{ 1,0,0 };
    playerUp = IPLVector3{ 0,1,0 };
}

SteamAudio::~SteamAudio() {

}

void SteamAudio::Process(IPLAudioBuffer input, IPLVector3 sourcePos, float sourceRadius) {
    IPLAudioBuffer processedFrame = renderers->Process(input, playerPos, playerDir, playerUp, sourcePos, sourceRadius);
    processedBuffers.push_back(processedFrame);
}

void SteamAudio::GetFinalMix(IPLAudioBuffer* finalBuf, uint32_t& numSamples) {
    // No audio is playing, return emptiness, let SoundManager figure it out.
    if (processedBuffers.size() == 0) {
        numSamples = 0;
        finalBuf = nullptr;
        return;
    }

    // Move the buffers into an array (needed for steam audios mixing)
    IPLAudioBuffer* buffers = new IPLAudioBuffer[processedBuffers.size()];
    for (std::size_t i = 0; i < processedBuffers.size(); i++) {
        buffers[i] = processedBuffers[i];
    }

    finalBuf->format = buffers[0].format;
    iplMixAudioBuffers(processedBuffers.size(), buffers, *finalBuf);
    numSamples = finalBuf->numSamples;

    // Clean up
    for (IPLAudioBuffer buf : processedBuffers) {
        if (buf.interleavedBuffer != nullptr)
            delete[] buf.interleavedBuffer;
    }
    processedBuffers.clear();
}

void SteamAudio::SetPlayer(IPLVector3 pos, IPLVector3 dir, IPLVector3 up) {
    playerPos = pos;
    playerDir = dir;
    playerUp = up;
}

void SteamAudio::CreateRenderers(IPLhandle environment) {
    directRenderer = new SteamAudioDirectRenderer{ environment };
    indirectRenderer = new SteamAudioIndirectRenderer{ environment };
}
