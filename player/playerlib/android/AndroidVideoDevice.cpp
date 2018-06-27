#include "AndroidVideoDevice.hpp"

namespace android {
	AndroidVideoDevice::AndroidVideoDevice(){}
	AndroidVideoDevice::~AndroidVideoDevice(){}

	// 获取绘制窗口
	//		
	FFL::sp<player::VideoSurface> AndroidVideoDevice::getSurface() {
		return 0;
	}
	//
	// 更新绘制的目标窗口
	//
	void AndroidVideoDevice::setSurface(SurfaceHandle surface) {

	}
	//
	//  打开关闭视频设备
	//
	bool AndroidVideoDevice::open(SurfaceHandle surface, int32_t widht, int32_t height) {
		return false;
	}
	void AndroidVideoDevice::close() {

	}
	//
	//  清空cache的数据
	//
	void AndroidVideoDevice::clearCache() {

	}
	//
	//  写一帧数据
	//		
	bool AndroidVideoDevice::showTexture(player::VideoTexture* texture) {
		return false;
	}
}