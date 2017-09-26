#pragma once
#include <phonon.h>

class SteamAudioDirectRenderer {
public:

	SteamAudioDirectRenderer();

	SteamAudioDirectRenderer(IPLContext context, IPLhandle* environment);

	~SteamAudioDirectRenderer();

	/// Processes the direct path portion of the audio
	/**
	* @param IPLAudioBuffer The audiobuffer to be processed.
	* @param IPLVector3* The position of the player in world space.
	* @param IPLVector3* The forward direction of the player.
	* @param IPLVector3* The up direction of the player.
	* @return IPLAudioBuffer The processed buffer.
	*/
	IPLAudioBuffer process(IPLAudioBuffer input, IPLVector3* playerPos, IPLVector3* PlayerDir, IPLVector3* playerUp);

private:
	IPLDirectSoundPath GetDirectSoundPath();
	IPLhandle* environment;
	IPLContext context;
};
