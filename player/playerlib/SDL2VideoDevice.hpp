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
		//  打开关闭视频设备
		//
		virtual bool open(void* wnd, int32_t widht, int32_t height);
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