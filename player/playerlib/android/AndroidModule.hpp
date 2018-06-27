#ifndef _ANDROID_MODULE_HPP
#define _ANDROID_MODULE_HPP

#include "DeviceFactory.hpp"
#include "AndroidAudioDevice.hpp"
#include "AndroidVideoDevice.hpp"

namespace android {
	class AndroidModule : public player::DeviceFactory {
	public:
		AndroidModule();
		~AndroidModule();
		//
		//  创建播放视频设备
		//
		virtual FFL::sp<player::VideoDevice> createVideoDevice(player::PlayerCore* player) ;
		//
		//  创建播放音频设备
		//
		virtual FFL::sp<player::AudioDevice> createAudioDevice(player::PlayerCore* player) ;

	private:
		FFL::sp<AndroidVideoDevice> mVideoDevice;
		FFL::sp<AndroidAudioDevice> mAudioDevice;
	};
}

#endif