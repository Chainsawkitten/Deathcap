#pragma once
#include "SteamAudioDirectRenderer.hpp"
#include "SteamAudioIndirectRenderer.hpp"


class SteamAudio {
public:
	/// Processes the audio with both indirect and direct methods and mixes them together.
	IPLAudioBuffer process(IPLAudioBuffer input, IPLVector3* playerPos, IPLVector3* PlayerDir, IPLVector3* playerUp, IPLVector3* sourcePos, float sourceRadius);

private:
	SteamAudioDirectRenderer directRenderer;
	SteamAudioIndirectRenderer indirectRenderer;
	IPLhandle* environmentalRenderer;
};