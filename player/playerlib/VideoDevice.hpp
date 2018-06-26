#ifndef _VIDEO_DEVICE_HPP_
#define _VIDEO_DEVICE_HPP_

#include <ref/FFL_Ref.hpp>
#include "VideoTexture.hpp"
#include "VideoSurface.hpp"

namespace player {
	class VideoRender;
	class VideoSurface;
	class VideoDevice : public FFL::RefBase{
	public:		
		VideoDevice();
		virtual ~VideoDevice();	
		//
		// 获取绘制窗口
		//		
		virtual FFL::sp<VideoSurface> getSurface()=0;
		//
		// 更新绘制的目标窗口
		//
		virtual void setSurface(SurfaceHandle surface)=0;
		//
		//  打开关闭视频设备
		//
		virtual bool open(SurfaceHandle surface,int32_t widht,int32_t height) = 0;
		virtual void close() = 0;
		//
		//  清空cache的数据
		//
		virtual void clearCache()=0;
		//
		//  写一帧数据
		//		
		virtual bool showTexture(VideoTexture* texture)=0;
		//
		// 获取render
		//
		virtual FFL::sp<VideoRender> getRender(void* userdata) ;		
	private:
		FFL::sp<VideoRender> mVideoRender;
	};
}
#endif
