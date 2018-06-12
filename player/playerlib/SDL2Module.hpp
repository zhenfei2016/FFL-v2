/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  SDL2Module.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/28
*
*  sdl2模块的相关封装
*
*/

#pragma once
#include "NodeBase.hpp"
#include "FFL_Texture.hpp"
#include <pipeline/FFL_PipelineMessageCache.hpp>
#include <SDL.h>
#include "FFL_Window.hpp"
#include "FFL_Sample.hpp"
#include "MessageSDL2Texture.hpp"
#include <utils/FFL_Utils.hpp>
#include <thread/FFL_Thread.hpp>


namespace player {
	class SDL2VideoDevice;
	class SDL2AudioDevice;
	
	class SDL2Module : public FFL::RefBase {
	public:
		SDL2Module();
		~SDL2Module();	
		
		
		bool create();
		void destroy();

		//
		// 通过纹理创建一个sdl2模块可以处理的消息
		//
		FFL::sp<FFL::PipelineMessage> createSDL2Texture(FFLTexture* texture);
		bool showSDL2Texture(FFL::sp<FFL::PipelineMessage> msg);
		bool showTexture(FFLTexture* texture);

		void addSample(FFLSample *samples);
		void closeVideoDevice();

		bool isDeviceClosed();

		SDL2AudioDevice* getAudioDevice() const {
			return mAudioDevice;
		}

		SDL2VideoDevice* getVideoDevice() const {
			return mWindow;
		}

	private:
		SDL2VideoDevice* mWindow;
		SDL2AudioDevice* mAudioDevice;		
		SDL_Renderer* mSDLRenderer;
		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;		 
		
	};


	class SDL2VideoDevice : public FFLWindow {
	public:
		SDL2VideoDevice();
		~SDL2VideoDevice();

	private:
		void create();

		//
		//  设置窗口的宽度，高度
		//
		virtual void setWindowSize(int32_t widht, int32_t height);
	public:
		SDL_Window* mWindow;
	};

	class SDL2AudioDevice {
	public:
		struct Param{
			//
			//  采样率，深度，声道数 ,一帧的采样数
			//
			uint32_t mFreq;
			uint32_t mFormat;
			uint32_t mChannels;
			uint32_t mSampleNum;
		};
	public:		
		SDL2AudioDevice();
		~SDL2AudioDevice();

		bool coverParams(FFLSample* samples,Param& params);
		//
		//  打开关闭音频设备
		//
		bool open(const Param& samples);
		void close();

		bool isClosed() const { return !mIsOpened; };
		//
		//  写一帧数据
		//
		bool writeFrame(FFLSample* samples);
		//
		//  获取播放设备硬件的延迟
		//
		int64_t getDeviceDelayUs() const;
		//
		// 获取缓冲的延迟时间
		//
		int64_t getCacheDelayUs();
		int64_t getCacheDelay(FFL::TimeBase& base);
		int64_t getCacheBytes();
	protected:		
		static void  fillAudio(void *userdata, Uint8 *stream, int len);
		void write(uint8_t *stream, uint32_t len);
	private:
		FFL::CMutex mLock;
		FFL::CCondition mCond;
		FFL::ByteStream* mByteStream;
		int32_t mIsOpened;
		//
		//  每秒播放的字节数
		//
		uint32_t mBytesPerSec;
		//
		//  当前打开的设备参数
		//
		Param mDeviceParam;
	};

	//返回false就会退出循环
	typedef bool (*FUNKeyPress)(void* userdata,int key);
	void SDL2Loop(FUNKeyPress fun, void* userdata);
}
