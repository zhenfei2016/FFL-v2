#include "VideoDevice.hpp"
#include "VideoRender.hpp"

namespace player {
	VideoDevice::VideoDevice() {
	}
	VideoDevice::~VideoDevice(){
	}
	//
	// 获取render
	//
	FFL::sp<VideoRender> VideoDevice::getRender(void* userdata) {
		if (mVideoRender.isEmpty()) {
			mVideoRender = new VideoRender(this);
		}
		return mVideoRender;
	}
}
