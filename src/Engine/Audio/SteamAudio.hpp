#pragma once
#include "SteamAudioDirectRenderer.hpp"
#include "SteamAudioIndirectRenderer.hpp"

//Note to self: Might just merge this shit with the interface class.
/// Main Steam Audio class for processing audio samples
class SteamAudio {
    public:

        SteamAudio();

        SteamAudio(IPLContext* context, IPLhandle* environment);

        ~SteamAudio();

    /// Processes the audio with both indirect and direct methods and mixes them together.
    /**
    * @param IPLAudioBuffer The audiobuffer to be processed.
    * @param IPLVector3* The position of the player in world space.
    * @param IPLVector3* The forward direction of the player.
    * @param IPLVector3* The up direction of the player.
    * @param IPLVector3* The position of the audio source.
    * @param float The radius of the source, for calculating occlusion.
    * @return IPLAudioBuffer The processed buffer.
    */
    IPLAudioBuffer Process(IPLAudioBuffer input, IPLVector3* playerPos, IPLVector3* PlayerDir, IPLVector3* playerUp, IPLVector3* sourcePos, float sourceRadius);

    private:

        IPLAudioBuffer MixAudio(IPLAudioBuffer direct, IPLAudioBuffer indirect);

        IPLContext* context;

        SteamAudioDirectRenderer directRenderer;
        SteamAudioIndirectRenderer indirectRenderer;
        IPLhandle* environmentalRenderer;
};
