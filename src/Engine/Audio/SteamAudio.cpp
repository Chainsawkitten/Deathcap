#include "SteamAudio.hpp"

SteamAudio::SteamAudio()
{
}

SteamAudio::SteamAudio(IPLContext * context, IPLhandle * environment)
{
}

SteamAudio::~SteamAudio()
{
}

void SteamAudio::Process(IPLAudioBuffer input, IPLVector3 * playerPos, IPLVector3 * playerDir, IPLVector3 * playerUp, IPLVector3 * sourcePos, float sourceRadius)
{
    // Direct Processing

    // Indirect Processing

    //MixAudio(direct, indirect)

    processedBuffers.push_back(input);

}

void SteamAudio::GetFinalMix(IPLAudioBuffer* finalBuf, size_t* numSamples)
{
    // No audio is playing, return emptiness, let SoundManager figure it out.
    if (processedBuffers.size() == 0)
    {
        *numSamples = 0;
        finalBuf = nullptr;
        return;
    }

    // Move the buffers into an array of contiguous memory (needed for steam audios mixing)
    IPLAudioBuffer* buffers = new IPLAudioBuffer[processedBuffers.size()];
    for (int i = 0; i < processedBuffers.size(); i++)
    {
        buffers[i] = processedBuffers[i];
    }

    finalBuf->format = buffers[0].format;
    finalBuf->interleavedBuffer = new float[buffers[0].numSamples];
    finalBuf->numSamples = buffers[0].numSamples;
    iplMixAudioBuffers(processedBuffers.size(), buffers, *finalBuf);
    *numSamples = finalBuf->numSamples;

    for (IPLAudioBuffer buf : processedBuffers)
    {
        if(buf.interleavedBuffer != nullptr)
            delete buf.interleavedBuffer;
    }
    processedBuffers.clear();
}
