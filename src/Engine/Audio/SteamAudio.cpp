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
    iplDestroyEnvironmentalRenderer(&envRenderer);
    iplDestroyBinauralRenderer(&binauralRenderer);
    for (auto& it : rendererMap)
        delete it.second;
}

void SteamAudio::Process(std::vector<SteamAudio::SoundSourceInfo>& inputs, IPLAudioBuffer& output) {

    std::vector<IPLAudioBuffer> audioBuffers;
    audioBuffers.resize(inputs.size());
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        SteamAudio::SoundSourceInfo& input = inputs[i];
        std::map<unsigned int, SteamAudioRenderers*>::iterator it = rendererMap.find(input.GUID);
        if (it == rendererMap.end())
            rendererMap[input.GUID] = new SteamAudioRenderers(environment, envRenderer, binauralRenderer);

        rendererMap[input.GUID]->Process(input.buffer, playerPos, playerDir, playerUp, input.position, input.radius, audioBuffers[i]);
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
    this->environment = environment;

    IPLRenderingSettings settings;
    settings.frameSize = CHUNK_SIZE;
    settings.samplingRate = SAMPLE_RATE;
    settings.convolutionType = IPL_CONVOLUTIONTYPE_PHONON;

    IPLAudioFormat outputFormat;
    outputFormat.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    outputFormat.channelLayout = IPL_CHANNELLAYOUT_STEREO;
    outputFormat.numSpeakers = 2;
    outputFormat.speakerDirections = NULL;
    outputFormat.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

    IPLerror err = iplCreateEnvironmentalRenderer(IPLContext{ nullptr,nullptr,nullptr }, environment, settings, outputFormat, NULL, NULL, &envRenderer);
    IPLHrtfParams params{ IPL_HRTFDATABASETYPE_DEFAULT, NULL, 0, nullptr, nullptr, nullptr };
    err = iplCreateBinauralRenderer(IPLContext{ nullptr, nullptr, nullptr }, settings, params, &binauralRenderer);
}
