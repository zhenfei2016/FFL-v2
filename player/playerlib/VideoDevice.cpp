#include "VideoDevice.hpp"
#include "VideoRender.hpp"

namespace player {
	VideoDevice::VideoDevice() {
	}
	VideoDevice::~VideoDevice(){
	}
	//
	//  打开关闭视频设备
	//
	bool VideoDevice::open(SurfaceHandle surface, int32_t widht, int32_t height) {
		return onOpen(surface, widht, height);
	}
	void VideoDevice::close() {
		onClose();
		mVideoRender = NULL;
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
	void VideoDevice::resetRender() {
		mVideoRender = NULL;
	}
}
