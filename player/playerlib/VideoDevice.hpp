#ifndef _VIDEO_DEVICE_HPP_
#define _VIDEO_DEVICE_HPP_

#include <ref/FFL_Ref.hpp>
#include "VideoTexture.hpp"

namespace player {
	class VideoRender;
	class VideoDevice : public FFL::RefBase{
	public:		
		VideoDevice();
		virtual ~VideoDevice();
	
		//
		//  打开关闭视频设备
		//
		virtual bool open(void* wnd,int32_t widht,int32_t height) = 0;
		virtual void close() = 0;
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