#pragma once

#include <phonon.h>
#include "../linking.hpp"

namespace Audio {

    /// Sample rate of sound files.
    const unsigned int SAMPLE_RATE = 44100U;

    /// Number of samples stored in each memory chunk.
    const unsigned int CHUNK_SIZE = SAMPLE_RATE / 15;

    /// Class responsible for calculating direct audio.
    class SteamAudioRenderers {
        public:
            /// Constructor.
            /**
             * @param environment Handle to the Environment object to use.
             * @param envRenderer Handle to the Environment Renderer object to use.
             * @param binauralRenderer Handle to the Binaural Renderer object to use.
             */
            ENGINE_API SteamAudioRenderers(IPLhandle environment, IPLhandle envRenderer, IPLhandle binauralRenderer);

            /// Destructor.
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
            IPLhandle directEffect;
            IPLhandle binauralEffect;
            IPLhandle convEffect;
            IPLAudioBuffer effectBuffer;
            IPLAudioBuffer directBuffer;
    };
}
