#include "AndroidModule.hpp"

namespace android {
	AndroidModule::AndroidModule()  {

	}
	AndroidModule::~AndroidModule() {

	}
	//
	//  创建播放视频设备
	//
	FFL::sp<player::VideoDevice> AndroidModule::createVideoDevice(player::PlayerCore* player) {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = new AndroidVideoDevice();
		}
		return mVideoDevice;
	}
	//
	//  创建播放音频设备
	//
	FFL::sp<player::AudioDevice> AndroidModule::createAudioDevice(player::PlayerCore* player) {
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = new AndroidAudioDevice();
		}
		return mAudioDevice;
	}



}
