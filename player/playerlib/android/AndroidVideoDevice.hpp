#ifndef _ANDROID_VIDEO_DEVICE_HPP_
#define _ANDROID_VIDEO_DEVICE_HPP_
#include "VideoDevice.hpp"
#include <FFL.h>
namespace android {
	class AndroidVideoDevice : public player::VideoDevice{
	public:
		AndroidVideoDevice();
		~AndroidVideoDevice();

	public:
		// 获取绘制窗口
		//		
		virtual FFL::sp<player::VideoSurface> getSurface() ;
		//
		// 更新绘制的目标窗口
		//
		virtual void setSurface(SurfaceHandle surface) ;
		//
		//  打开关闭视频设备
		//
		virtual bool open(SurfaceHandle surface, int32_t widht, int32_t height) ;
		virtual void close() ;
		//
		//  清空cache的数据
		//
		virtual void clearCache() ;
		//
		//  写一帧数据
		//		
		virtual bool showTexture(player::VideoTexture* texture) ;
	};
}

#endif