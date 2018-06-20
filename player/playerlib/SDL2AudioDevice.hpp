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

#ifndef _AUDIO_SDL2_DEVICE_HPP_
#define _AUDIO_SDL2_DEVICE_HPP_


#include "NodeBase.hpp"
#include "VideoTexture.hpp"
#include <pipeline/FFL_PipelineMessageCache.hpp>
#include <SDL.h>
#include "VideoSurface.hpp"
#include "AudioSample.hpp"
#include "MessageSDL2Texture.hpp"
#include <utils/FFL_Utils.hpp>
#include <utils/FFL_List.hpp>
#include <thread/FFL_Thread.hpp>
#include "AudioDevice.hpp"


namespace player {
	class SDL2AudioDevice :public AudioDevice {		
	public:		
		SDL2AudioDevice();
		~SDL2AudioDevice();

		//
		//  获取支持的格式
		//  wanted: 如果为nUll则返回所有支持的格式
		//           非null 返回跟他匹配的 
		//  fmtList: 返回支持的格式list
		//
		virtual void getSupportFormat(const AudioFormat* wanted, FFL::List<AudioFormat>& fmtList) ;
		virtual bool isSupportFormat(const AudioFormat* wanted) ;
		//
		// 设置缓冲多长时间的数据
		//
		virtual void setCacheUs(int64_t us);
		//
		//  打开关闭音频设备
		//
		virtual bool open(const AudioFormat& wanted, int32_t sampleNum, AudioFormat& obtained) ;
		virtual void close() ;
		//
		//获取当前打开的格式，如果未打开则返回null
		//
		virtual AudioFormat* getOpenFormat();
		//
		//  写一帧数据
		//
		virtual bool writeFrame(AudioSample* samples) ;
		//
		//  获取播放设备硬件的延迟
		//
		virtual int64_t getDeviceDelayUs() ;
		//
		// 获取缓冲的数据量，还没有播放
		//
		virtual int64_t getCacheBytes() ;
		//
		// 获取缓冲的延迟时间
		//
		int64_t getCacheDelayUs();
		int64_t getCacheDelay(FFL::TimeBase& base);		
		//
		// 获取播放中的音频的pts
		//
		int64_t getRenderingPts();
		bool isOpened() const { return mIsOpened; };
	private:
		//
		//  获取相似的音频格式
		//
		bool getSimilarFormat(const AudioFormat* wanted, AudioFormat& obtained);
	protected:		
		//
		//  sdl2底层回调的填充数据
		//
		static void  SDL2_fillAudio(void *userdata, Uint8 *stream, int len);
		void SDL2_fill(uint8_t *stream, uint32_t len);
		//
		//  从本地缓存读数据到交换缓冲中
		//
		uint32_t readData2SwapBuffer(uint32_t wantedSize);
	private:
		FFL::CMutex mLock;
		FFL::CCondition mCond;
		FFL::ByteBuffer* mByteBuffer;
		bool mIsOpened;
		int64_t mCacheUs;
		//
		//  每秒播放的字节数
		//
		uint32_t mBytesPerSec;
		//
		//  当前打开的设备参数
		//
		AudioFormat mObtainedFormat;

		int64_t mDeviceDelayUs;
		//
		// 用于sdl2和mByteBuffer间交换数据的
		//
		FFL::ByteBuffer mSwapBuffer;

	private:
		struct SampleEntry {
			int64_t pts;
			int64_t size;

			int64_t consumeCount;
		};
		FFL::List<SampleEntry> mSamples;
		int64_t mCachedBytes;
		int64_t mCurrentRenderPts;

		
	};
}
#endif