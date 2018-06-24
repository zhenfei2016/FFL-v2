#ifndef _SDL2_VIDEO_DEVICE_HPP_
#define _SDL2_VIDEO_DEVICE_HPP_

#include <SDL.h>
#include "VideoDevice.hpp"

#include <pipeline/FFL_PipelineMessage.hpp>
#include <pipeline/FFL_PipelineMessageCache.hpp>

namespace player {
	class VideoRender;
	class SDL2VideoDevice :public VideoDevice {		
	public:		
		SDL2VideoDevice();
		~SDL2VideoDevice();
		//
		// 获取绘制窗口
		//		
		virtual FFL::sp<VideoSurface> getSurface();
		//
		// 更新绘制的目标窗口
		//
		virtual void setSurface(SurfaceHandle surface);
		//
		//  打开关闭视频设备
		//
		virtual bool open(SurfaceHandle wnd, int32_t widht, int32_t height);
		virtual void close();
		//
		//  显示当前文理
		//
		virtual bool showTexture(VideoTexture* texture);
	protected:
		//
		// 通过纹理创建一个sdl2模块可以处理的消息
		//
		FFL::sp<FFL::PipelineMessage> createSDL2Texture(VideoTexture* texture);
		bool showSDL2Texture(FFL::sp<FFL::PipelineMessage> msg);

	private:
		FFL::sp<VideoSurface> mSurface;
		SDL_Window* mWindow;
		SDL_Renderer* mSDLRenderer;

		FFL::sp<VideoRender> mVideoRender;
		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
	};
}
#endif