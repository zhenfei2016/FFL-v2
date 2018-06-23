#ifndef _VIDEO_DEVICE_HPP_
#define _VIDEO_DEVICE_HPP_

#include <ref/FFL_Ref.hpp>
#include "VideoTexture.hpp"

namespace player {
	class VideoRender;
	class VideoSurface;
	class VideoDevice : public FFL::RefBase{
	public:		
		VideoDevice();
		virtual ~VideoDevice();	
		//
		// ��ȡ���ƴ���
		//		
		virtual FFL::sp<VideoSurface> getSurface()=0;
		//
		// ���»��Ƶ�Ŀ�괰��
		//
		virtual void setSurface(FFL::sp<VideoSurface> surface)=0;
		//
		//  �򿪹ر���Ƶ�豸
		//
		virtual bool open(FFL::sp<VideoSurface> surface,int32_t widht,int32_t height) = 0;
		virtual void close() = 0;
		//
		//  дһ֡����
		//		
		virtual bool showTexture(VideoTexture* texture)=0;
		//
		// ��ȡrender
		//
		virtual FFL::sp<VideoRender> getRender(void* userdata) ;		
	private:
		FFL::sp<VideoRender> mVideoRender;
	};
}
#endif