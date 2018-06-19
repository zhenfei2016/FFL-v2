#include "AudioDevice.hpp"
#include "AudioRender.hpp"
namespace player {
	AudioDevice::AudioDevice() {
	}
	AudioDevice::~AudioDevice(){
	}
	//
	// ªÒ»°render
	//
	FFL::sp<AudioRender> AudioDevice::getRender(void* userdata) {
		if (mAudioRender.isEmpty()) {
			mAudioRender = new AudioRender(this);
		}
		return mAudioRender;
	}
}