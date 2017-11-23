#include "SteamAudioRenderers.hpp"

SteamAudioRenderers::SteamAudioRenderers(IPLhandle environment, IPLhandle envRenderer, IPLhandle binauralRenderer) {
    this->environment = environment;

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

    // Direct Sound Effect.
    iplCreateDirectSoundEffect(envRenderer, inputFormat, inputFormat, &directEffect);

    // Binaural Renderer and Effect.
    iplCreateBinauralEffect(binauralRenderer, inputFormat, outputFormat, &binauralEffect);

    // Convolution Effect
    iplCreateConvolutionEffect(envRenderer, "", IPL_SIMTYPE_REALTIME, inputFormat, outputFormat, &convEffect);

    effectBuffer.format = inputFormat;
    effectBuffer.numSamples = CHUNK_SIZE;
    effectBuffer.interleavedBuffer = new float[CHUNK_SIZE];
    effectBuffer.deinterleavedBuffer = NULL;
}

SteamAudioRenderers::~SteamAudioRenderers() {
    iplDestroyDirectSoundEffect(&directEffect);
    iplDestroyBinauralEffect(&binauralEffect);
    iplDestroyConvolutionEffect(&convEffect);

    delete[] effectBuffer.interleavedBuffer;
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

    output.format = outputFormat;
    output.numSamples = CHUNK_SIZE;
    output.interleavedBuffer = new float[CHUNK_SIZE * 2];
    output.deinterleavedBuffer = NULL;

    // Spatialize the direct audio
    iplApplyBinauralEffect(binauralEffect, effectBuffer, soundPath.direction, IPL_HRTFINTERPOLATION_BILINEAR, output);

    // Indirect Audio
    iplSetDryAudioForConvolutionEffect(convEffect, sourcePos, input);
}
