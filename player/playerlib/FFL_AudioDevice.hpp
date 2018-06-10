#pragma once
#include <ref/FFL_Ref.hpp>
class FFLAudioDevice : public FFL::RefBase
{
public:
	FFLAudioDevice();
	~FFLAudioDevice();

	uint32_t mFormat;
	uint32_t mChannel;
	uint32_t mFreq;
};