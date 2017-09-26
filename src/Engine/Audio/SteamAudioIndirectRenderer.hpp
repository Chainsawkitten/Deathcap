#pragma once
#include <phonon.h>

class SteamAudioIndirectRenderer {
public:
	SteamAudioIndirectRenderer();

	SteamAudioIndirectRenderer(IPLhandle* environment, IPLContext context);

	~SteamAudioIndirectRenderer();


	IPLAudioBuffer process(IPLAudioBuffer input);

private:
	/// Creates the binaural renderer object.
	void CreateBinauralRenderer();
	IPLhandle* environment;
	IPLContext context;
};
