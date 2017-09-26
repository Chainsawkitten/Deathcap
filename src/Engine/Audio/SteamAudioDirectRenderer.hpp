#pragma once
#include <phonon.h>

/// Class responsible for calculating direct audio.
class SteamAudioDirectRenderer {
    public:

        SteamAudioDirectRenderer();

        /**
         * @param IPLContext The Steam Audio context.
         * @param IPLhandle* Handle to the Environment object to use.
         */
        SteamAudioDirectRenderer(IPLContext context, IPLhandle* environment);

        ~SteamAudioDirectRenderer();

        /// Processes the direct path portion of the audio
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
        IPLDirectSoundPath GetDirectSoundPath();
        IPLhandle* environment;
        IPLContext context;
};
