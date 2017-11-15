#include "SteamAudioDirectRenderer.hpp"

SteamAudioDirectRenderer::SteamAudioDirectRenderer() {
    directEffect = nullptr;
    envRenderer = nullptr;
}

SteamAudioDirectRenderer::SteamAudioDirectRenderer(IPLhandle environment) {
    this->environment = environment;
    this->envRenderer = new IPLhandle;
    this->directEffect = new IPLhandle;

    IPLRenderingSettings settings;
    settings.frameSize = 1024;
    settings.samplingRate = 44100;
    settings.convolutionType = IPL_CONVOLUTIONTYPE_PHONON;

    inputFormat.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    inputFormat.channelLayout = IPL_CHANNELLAYOUT_MONO; //Only take mono input
    inputFormat.numSpeakers = 1;
    inputFormat.speakerDirections = NULL;
    inputFormat.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

    outputFormat.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    outputFormat.channelLayout = IPL_CHANNELLAYOUT_STEREO;
    outputFormat.numSpeakers = 2;
    outputFormat.speakerDirections = NULL;
    outputFormat.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

    IPLerror err = iplCreateEnvironmentalRenderer(IPLContext{ nullptr,nullptr,nullptr }, environment, settings, outputFormat, NULL, NULL, envRenderer);

    iplCreateDirectSoundEffect(*envRenderer, inputFormat, outputFormat, directEffect);
}

SteamAudioDirectRenderer::~SteamAudioDirectRenderer() {
    if (directEffect != nullptr)
        iplDestroyDirectSoundEffect(directEffect);
    if (envRenderer != nullptr)
        iplDestroyEnvironmentalRenderer(envRenderer);
}

IPLAudioBuffer SteamAudioDirectRenderer::Process(IPLAudioBuffer input, IPLVector3 playerPos, IPLVector3 playerDir, IPLVector3 playerUp, IPLVector3 sourcePos, float sourceRadius)
{
    IPLDirectSoundPath soundPath = iplGetDirectSoundPath(environment, playerPos, playerDir, playerUp, sourcePos, sourceRadius, IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY, IPL_DIRECTOCCLUSION_VOLUMETRIC);

    IPLDirectSoundEffectOptions options;
    options.applyAirAbsorption = (IPLbool)true;
    options.applyDistanceAttenuation = (IPLbool)true;
    options.directOcclusionMode = IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY;

    IPLAudioBuffer outputBuffer;
    outputBuffer.format = outputFormat;
    outputBuffer.numSamples = 1024;
    outputBuffer.interleavedBuffer = new float[2048];
    outputBuffer.deinterleavedBuffer = NULL;

    iplApplyDirectSoundEffect(*directEffect, input, soundPath, options, outputBuffer);
    return outputBuffer;
}
