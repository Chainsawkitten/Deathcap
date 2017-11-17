#include "SteamAudioRenderers.hpp"

SteamAudioRenderers::SteamAudioRenderers() {
    directEffect = nullptr;
    envRenderer = nullptr;
    binauralRenderer = nullptr;
    binauralEffect = nullptr;
}

SteamAudioRenderers::SteamAudioRenderers(IPLhandle environment) {
    this->environment = environment;
    this->envRenderer = new IPLhandle;
    this->directEffect = new IPLhandle;
    this->binauralRenderer = new IPLhandle;
    this->binauralEffect = new IPLhandle;
    this->convEffect = new IPLhandle;

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
    IPLerror err = iplCreateEnvironmentalRenderer(IPLContext{ nullptr,nullptr,nullptr }, environment, settings, outputFormat, NULL, NULL, envRenderer);
    iplCreateDirectSoundEffect(*envRenderer, inputFormat, inputFormat, directEffect);

    // Binaural Renderer and Effect
    IPLHrtfParams params{IPL_HRTFDATABASETYPE_DEFAULT, NULL, 0, nullptr, nullptr, nullptr };    //Might not work
    err = iplCreateBinauralRenderer(IPLContext{ nullptr, nullptr, nullptr }, settings, params, binauralRenderer);
    iplCreateBinauralEffect(*binauralRenderer, inputFormat, outputFormat, binauralEffect);

    // Convolution Effect
    iplCreateConvolutionEffect(*envRenderer, "", IPL_SIMTYPE_REALTIME, inputFormat, outputFormat, convEffect);
}

SteamAudioRenderers::~SteamAudioRenderers() {
    if (directEffect != nullptr)
        iplDestroyDirectSoundEffect(directEffect);
    if (envRenderer != nullptr)
        iplDestroyEnvironmentalRenderer(envRenderer);
    if (binauralRenderer != nullptr)
        iplDestroyBinauralRenderer(binauralRenderer);
    if (binauralEffect != nullptr)
        iplDestroyBinauralEffect(binauralEffect);
    if (convEffect != nullptr)
        iplDestroyConvolutionEffect(convEffect);
}

IPLAudioBuffer SteamAudioRenderers::Process(IPLAudioBuffer input, IPLVector3 playerPos, IPLVector3 playerDir, IPLVector3 playerUp, IPLVector3 sourcePos, float sourceRadius)
{
    IPLAudioBuffer* finalBuffers = new IPLAudioBuffer[2];

    // Calculate direct sound path
    IPLDirectSoundPath soundPath = iplGetDirectSoundPath(environment, playerPos, playerDir, playerUp, sourcePos, sourceRadius, IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY, IPL_DIRECTOCCLUSION_VOLUMETRIC);

    // Calculate direct effect (occlusion & attenuation)
    IPLDirectSoundEffectOptions options;
    options.applyAirAbsorption = (IPLbool)false;
    options.applyDistanceAttenuation = (IPLbool)false;
    options.directOcclusionMode = IPL_DIRECTOCCLUSION_TRANSMISSIONBYFREQUENCY;

    IPLAudioBuffer effectBuffer;
    effectBuffer.format = inputFormat;
    effectBuffer.numSamples = CHUNK_SIZE;
    effectBuffer.interleavedBuffer = new float[CHUNK_SIZE];
    effectBuffer.deinterleavedBuffer = NULL;

    iplApplyDirectSoundEffect(*directEffect, input, soundPath, options, effectBuffer);
    delete[] effectBuffer.interleavedBuffer;

    // Spatialize the direct audio
    finalBuffers[0].format = outputFormat;
    finalBuffers[0].numSamples = CHUNK_SIZE;
    finalBuffers[0].interleavedBuffer = new float[CHUNK_SIZE*2];
    finalBuffers[0].deinterleavedBuffer = NULL;

    iplApplyBinauralEffect(*binauralEffect, input, soundPath.direction, IPL_HRTFINTERPOLATION_BILINEAR, finalBuffers[0]);
    
    // TMPTODO
    //// Indirect Audio
    //iplSetDryAudioForConvolutionEffect(*convEffect, sourcePos, input);
    //finalBuffers[1].format = outputFormat;
    //finalBuffers[1].numSamples = CHUNK_SIZE;
    //finalBuffers[1].interleavedBuffer = new float[CHUNK_SIZE*2];
    //finalBuffers[1].deinterleavedBuffer = NULL;
    //iplGetWetAudioForConvolutionEffect(*convEffect, playerPos, playerDir, playerUp, finalBuffers[1]);

    //// Mix Direct and Indirect
    //IPLAudioBuffer mixedBuffer;
    //mixedBuffer.format = outputFormat;
    //mixedBuffer.numSamples = CHUNK_SIZE;
    //mixedBuffer.interleavedBuffer = new float[CHUNK_SIZE*2];
    //mixedBuffer.deinterleavedBuffer = NULL;

    //iplMixAudioBuffers(2, finalBuffers, mixedBuffer);
       
    //delete[] finalBuffers[0].interleavedBuffer;
    //delete[] finalBuffers[1].interleavedBuffer;
    //delete[] finalBuffers;
    // NEEED TOOO DELEEETE BUFFEEEERS THAAAT ARRRE NOOOOT UUUSED ANNYYYY MOOORREE
    return finalBuffers[0];
}
