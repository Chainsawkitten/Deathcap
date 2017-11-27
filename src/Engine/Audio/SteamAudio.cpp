#include "SteamAudio.hpp"
#include <assert.h>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Audio;

SteamAudio::SteamAudio() {
    playerPos = IPLVector3{ 0, 0, 0 };
    playerDir = IPLVector3{ 1, 0, 0 };
    playerUp = IPLVector3{ 0, 1, 0 };

    outputFormat.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    outputFormat.channelLayout = IPL_CHANNELLAYOUT_STEREO;
    outputFormat.numSpeakers = 2;
    outputFormat.speakerDirections = NULL;
    outputFormat.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

    indirectBuffer.format = outputFormat;
    indirectBuffer.numSamples = CHUNK_SIZE;
    indirectBuffer.interleavedBuffer = new float[CHUNK_SIZE * 2];
    indirectBuffer.deinterleavedBuffer = NULL;

    directBuffer.format = outputFormat;
    directBuffer.numSamples = CHUNK_SIZE;
    directBuffer.interleavedBuffer = new float[CHUNK_SIZE * 2];
    directBuffer.deinterleavedBuffer = NULL;
}

SteamAudio::~SteamAudio() {
    Clear();

    delete[] indirectBuffer.interleavedBuffer;
    delete[] directBuffer.interleavedBuffer;
}

void SteamAudio::Process(std::vector<SteamAudio::SoundSourceInfo>& inputs, IPLAudioBuffer& output) {

    std::vector<IPLAudioBuffer> audioBuffers;
    audioBuffers.resize(inputs.size());
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        SteamAudio::SoundSourceInfo& input = inputs[i];
        audioBuffers[i] = input.renderers->Process(input.buffer, playerPos, playerDir, playerUp, input.position, input.radius);
    }

    assert(!audioBuffers.empty());
    output.format = audioBuffers[0].format;
    iplMixAudioBuffers(audioBuffers.size(), audioBuffers.data(), directBuffer);
    iplGetMixedEnvironmentalAudio(envRenderer, playerPos, playerDir, playerUp, indirectBuffer);
    std::vector<IPLAudioBuffer> mixBuffers{ directBuffer, indirectBuffer };
    iplMixAudioBuffers(mixBuffers.size(), mixBuffers.data(), output);
}

void SteamAudio::SetPlayer(IPLVector3 pos, IPLVector3 dir, IPLVector3 up) {
    playerPos = pos;
    playerDir = dir;
    playerUp = up;
}

void SteamAudio::CreateRenderers(IPLhandle environment) {
    Clear();

    this->environment = environment;

    IPLRenderingSettings settings;
    settings.frameSize = CHUNK_SIZE;
    settings.samplingRate = SAMPLE_RATE;
    settings.convolutionType = IPL_CONVOLUTIONTYPE_PHONON;

    IPLerror err = iplCreateEnvironmentalRenderer(IPLContext{ nullptr, nullptr, nullptr }, environment, settings, outputFormat, NULL, NULL, &envRenderer);
    IPLHrtfParams params{ IPL_HRTFDATABASETYPE_DEFAULT, NULL, 0, nullptr, nullptr, nullptr };
    err = iplCreateBinauralRenderer(IPLContext{ nullptr, nullptr, nullptr }, settings, params, &binauralRenderer);
}

void SteamAudio::Clear() {
    if (envRenderer) {
        iplDestroyEnvironmentalRenderer(&envRenderer);
        envRenderer = NULL;
    }
        
    if (binauralRenderer) {
        iplDestroyBinauralRenderer(&binauralRenderer);
        binauralRenderer = NULL;
    }
}
