#pragma once

#include <phonon.h>
#include "../linking.hpp"

/// Class responsible for calculating direct audio.
class SteamAudioRenderers {
    public:

        ENGINE_API SteamAudioRenderers();

        /**
         * @param environment Handle to the Environment object to use.
         */
        ENGINE_API SteamAudioRenderers(IPLhandle environment);

        ENGINE_API ~SteamAudioRenderers();

        /// Processes the direct path portion of the audio
        /**
         * @param input The audiobuffer to be processed.
         * @param playerPos The position of the player in world space.
         * @param playerDir The forward direction of the player.
         * @param playerUp The up direction of the player.
         * @param sourcePos The position of the audio source.
         * @param sourceRadius The radius of the source, for calculating occlusion.
         * @return The processed buffer.
         */
        ENGINE_API IPLAudioBuffer Process(IPLAudioBuffer input, IPLVector3 playerPos, IPLVector3 playerDir, IPLVector3 playerUp, IPLVector3 sourcePos, float sourceRadius);

    private:
        IPLAudioFormat inputFormat;
        IPLAudioFormat outputFormat;
        IPLhandle environment;
        IPLhandle* envRenderer;
        IPLhandle* directEffect;
        IPLhandle* binauralRenderer;
        IPLhandle* binauralEffect;
        IPLhandle* convEffect;
};
