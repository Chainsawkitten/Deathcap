#pragma once

#include <phonon.h>
#include "../linking.hpp"

/// Class responsible for calculating indirect audio.
class SteamAudioIndirectRenderer {
    public:

        ENGINE_EXPORT SteamAudioIndirectRenderer();

        /**
         * @param context The Steam Audio context.
         * @param environment Handle to the Environment object to use.
         */
        ENGINE_EXPORT SteamAudioIndirectRenderer(IPLContext* context, IPLhandle* environment);

        ENGINE_EXPORT ~SteamAudioIndirectRenderer();

        /// Processes the indirect portion of the audio.
        /**
        * @param input The audiobuffer to be processed.
        * @param direction The unit vector from the listener to the point source, relative to the listener's coordinate system.
        * @return The processed buffer.
        */
        ENGINE_EXPORT IPLAudioBuffer Process(IPLAudioBuffer input, IPLVector3 direction);

    private:
        /// Creates the binaural renderer object.
        void CreateBinauralRenderer();
        IPLhandle* environment;
        IPLContext* context;
};
