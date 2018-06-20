#ifndef _AVDEVICE_CREATOR_HPP_
#define _AVDEVICE_CREATOR_HPP_

#include <ref/FFL_Ref.hpp>
namespace player {
	class VideoDevice;
	class AudioDevice;
	class PlayerCore;
	class AVDeviceCreator {
	public:
		AVDeviceCreator();
		virtual ~AVDeviceCreator();

		//
		//  创建播放视频设备
		//
		virtual FFL::sp<VideoDevice> createVideoDevice(PlayerCore* player)=0;
		//
		//  创建播放音频设备
		//
		virtual FFL::sp<AudioDevice> createAudioDevice(PlayerCore* player)=0;
	};
}
#endif
