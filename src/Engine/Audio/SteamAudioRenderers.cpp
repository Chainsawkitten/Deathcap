#include "SteamAudioRenderers.hpp"

SteamAudioRenderers::SteamAudioRenderers(IPLhandle environment) {
    this->environment = environment;

    IPLRenderingSettings settings;
    settings.frameSize = CHUNK_SIZE;
    settings.samplingRate = SAMPLE_RATE;
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

    // Environmental Renderer and Direct Sound Effect
    IPLerror err = iplCreateEnvironmentalRenderer(IPLContext{ nullptr,nullptr,nullptr }, environment, settings, outputFormat, NULL, NULL, &envRenderer);
    iplCreateDirectSoundEffect(envRenderer, inputFormat, inputFormat, &directEffect);

    // Binaural Renderer and Effect
    IPLHrtfParams params{IPL_HRTFDATABASETYPE_DEFAULT, NULL, 0, nullptr, nullptr, nullptr };
    err = iplCreateBinauralRenderer(IPLContext{ nullptr, nullptr, nullptr }, settings, params, &binauralRenderer);
    iplCreateBinauralEffect(binauralRenderer, inputFormat, outputFormat, &binauralEffect);

    // Convolution Effect
    err = iplCreateConvolutionEffect(envRenderer, "", IPL_SIMTYPE_REALTIME, inputFormat, outputFormat, &convEffect);

    effectBuffer.format = inputFormat;
    effectBuffer.numSamples = CHUNK_SIZE;
    effectBuffer.interleavedBuffer = new float[CHUNK_SIZE];
    effectBuffer.deinterleavedBuffer = NULL;

    finalBuffers[0].format = outputFormat;
    finalBuffers[0].numSamples = CHUNK_SIZE;
    finalBuffers[0].interleavedBuffer = new float[CHUNK_SIZE * 2];
    finalBuffers[0].deinterleavedBuffer = NULL;

    finalBuffers[1].format = outputFormat;
    finalBuffers[1].numSamples = CHUNK_SIZE;
    finalBuffers[1].interleavedBuffer = new float[CHUNK_SIZE * 2];
    finalBuffers[1].deinterleavedBuffer = NULL;
}

SteamAudioRenderers::~SteamAudioRenderers() {
    iplDestroyDirectSoundEffect(&directEffect);
    iplDestroyEnvironmentalRenderer(&envRenderer);
    iplDestroyBinauralRenderer(&binauralRenderer);
    iplDestroyBinauralEffect(&binauralEffect);
    iplDestroyConvolutionEffect(&convEffect);

    delete[] effectBuffer.interleavedBuffer;
    delete[] finalBuffers[0].interleavedBuffer;
    delete[] finalBuffers[1].interleavedBuffer;
}

void SteamAudioRenderers::Process(IPLAudioBuffer input, IPLVector3 playerPos, IPLVector3 playerDir, IPLVector3 playerUp, IPLVector3 sourcePos, float sourceRadius, IPLAudioBuffer& output) {
    // Calculate direct sound path
    IPLDirectSoundPath soundPath = iplGetDirectSoundPath(environment, playerPos, playerDir, playerUp, sourcePos, sourceRadius, IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY, IPL_DIRECTOCCLUSION_VOLUMETRIC);

    // Calculate direct effect (occlusion & attenuation)
    IPLDirectSoundEffectOptions options;
    options.applyAirAbsorption = (IPLbool)true;
    options.applyDistanceAttenuation = (IPLbool)true;
    options.directOcclusionMode = IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY;

    iplApplyDirectSoundEffect(directEffect, input, soundPath, options, effectBuffer);

    // Spatialize the direct audio
    iplApplyBinauralEffect(binauralEffect, effectBuffer, soundPath.direction, IPL_HRTFINTERPOLATION_BILINEAR, finalBuffers[0]);

    // Indirect Audio
    iplSetDryAudioForConvolutionEffect(convEffect, sourcePos, input);
    iplGetWetAudioForConvolutionEffect(convEffect, playerPos, playerDir, playerUp, finalBuffers[1]);

    // Mix Direct and Indirect
    output.format = outputFormat;
    output.numSamples = CHUNK_SIZE;
    output.interleavedBuffer = new float[CHUNK_SIZE * 2];
    output.deinterleavedBuffer = NULL;

    // Mix Direct and Indirect
    iplMixAudioBuffers(2, finalBuffers, output);
}
