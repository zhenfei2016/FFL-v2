/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  SDL2Module.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/04/28 
 *  
 *  sdl2模块的相关封装
 *
*/
#include "SDL2Module.hpp"
#include <SDL.h>

#include "SDL2Module.hpp"
#include "MessageSDL2Texture.hpp"
#include "FFL_AudioDevice.hpp"
#include "FFL_Player.hpp"
#include "FFMpeg.hpp"
#include "LogtagConstant.hpp"

namespace player {
	SDL2Module::SDL2Module() :mSDLRenderer(NULL) {				
		mWindow = NULL;
		mMessageCache = new FFL::PipelineMessageCache(MSG_SDL2_TEXTURE);
		mAudioDevice = new SDL2AudioDevice();
	}
	SDL2Module::~SDL2Module() {
		FFL_SafeFree(mAudioDevice);
	}
	bool SDL2Module::create() {
		mWindow = new SDL2VideoDevice();		
		return true;
	}
	void SDL2Module::destroy() {
		SDL_PauseAudio(1);
		SDL_CloseAudio();
		if (mSDLRenderer) {
			SDL_DestroyRenderer(mSDLRenderer);
			mSDLRenderer = 0;
		}
	}

	FFL::sp<FFL::PipelineMessage> SDL2Module::createSDL2Texture(FFLTexture* texture)
	{
		if (mSDLRenderer == NULL) {
			mSDLRenderer = SDL_CreateRenderer(mWindow->mWindow, 0, 0);			
		}

		if (mSDLRenderer == NULL) {
			return NULL;
		}

		message::SDL2Texture* sdlTexture = 0;
		FFL::sp<FFL::PipelineMessage> msg = message::createMessageFromCache(mMessageCache, &sdlTexture, MSG_SDL2_TEXTURE);

		uint32_t pixformat = 0;
		//IYUV: Y + U + V  (3 planes)
		//YV12: Y + V + U  (3 planes)
		pixformat = SDL_PIXELFORMAT_IYUV;

		SDL_Texture* pic = sdlTexture->mTexture;
		if (pic == NULL) {
			pic = SDL_CreateTexture(mSDLRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, texture->mWidth, texture->mHeight);
			sdlTexture->mTexture = pic;
		}


		SDL_UpdateYUVTexture(sdlTexture->mTexture, NULL,
			texture->mPixels[0], texture->mPitches[0],
			texture->mPixels[1], texture->mPitches[1],
			texture->mPixels[2], texture->mPitches[2]);
		return msg;
	}
	bool SDL2Module::showSDL2Texture(FFL::sp<FFL::PipelineMessage> msg) {

		if (msg.isEmpty()) {
			return false;
		}
		
		if (msg->getType() == MSG_SDL2_TEXTURE)
		{
			SDL_Texture * texture=((message::SDL2Texture*) msg->getPayload())->mTexture;

			//
			//清空渲染器
			//复制数据纹理给渲染器
			//显示
			//
			SDL_RenderClear(mSDLRenderer);
			SDL_RenderCopy(mSDLRenderer, texture, NULL, NULL);
			//SDL_SetRenderDrawColor(mSDLRenderer, 255,255, 0, 255);
			//SDLTest_DrawString(mSDLRenderer, 0, 0, "Hi");//
			SDL_RenderPresent(mSDLRenderer);
		}

		msg->consume(this);
		return true;
	}
	bool SDL2Module::showTexture(FFLTexture* texture) {
		FFL::sp<FFL::PipelineMessage> msg = createSDL2Texture(texture);
		return showSDL2Texture(msg);
	}
	void SDL2Module::closeVideoDevice() {
		SDL_DestroyRenderer(mSDLRenderer);
		SDL_DestroyWindow(mWindow->mWindow);
		mSDLRenderer = 0;
	}
	bool SDL2Module::isDeviceClosed() {
		return mSDLRenderer == 0 && mAudioDevice->isClosed();
	}
	void SDL2Module::addSample(FFLSample *samples)
	{
		if (mAudioDevice->isClosed()) {
			SDL2AudioDevice::Param params = {};
			if (!mAudioDevice->coverParams(samples, params))
			{
				FFL_LOG_WARNING("Failed to open SDL audio");
				return ;
			}
			mAudioDevice->open(params);
		}		
		mAudioDevice->writeFrame(samples);
	}		

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	SDL2VideoDevice::SDL2VideoDevice() {
		create();
	}
	SDL2VideoDevice::~SDL2VideoDevice() {

	}
	void SDL2VideoDevice::create() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {			
			return;
		}

		int widht = 400;
		int height = 300;
		SDL_Window *screen;
		//SDL 2.0 Support for multiple windows
		//画一个窗口，大小为screen_w * screen_h
		screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			widht, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!screen) {
			printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
			return;
		}
		mWindow = screen;
	}

	//
	//  设置窗口的宽度，高度
	//
	void SDL2VideoDevice::setWindowSize(int32_t widht, int32_t height) {
		SDL_SetWindowSize(mWindow,widht,height);
	}

	FFL::sp<FFLWindow> createSDL2RenderWindow() {
		return new SDL2VideoDevice();
	}

	void SDL2Loop(FUNKeyPress fun,void* userdata) {
		bool bQuit=false;
		SDL_Event event;
		//while (SDL_WaitEventTimeout(&event,40) && !bQuit) {
		while (SDL_WaitEvent(&event) && !bQuit) {
			switch (event.type) {
			case SDL_KEYUP:
				if (fun && !fun(userdata, event.key.keysym.sym))
					bQuit = true;
				break;
			case SDL_QUIT:
				bQuit = true;
				break;
			}
		}
	}


	int8_t* g_TempBuffer = NULL;
	uint32_t g_TempBufferLen = 0;
	SDL2AudioDevice::SDL2AudioDevice() :
         mByteStream(NULL),mIsOpened(false){
		mBytesPerSec = 1;
	}
	SDL2AudioDevice::~SDL2AudioDevice() {
		close();
		FFL_SafeFree(mByteStream);
	}

	struct FormatMap {
		int ffmpegFormat;
		int sdlFormat;
	};
	bool SDL2AudioDevice::coverParams(FFLSample* samples, Param& params) {
		params.mFreq = samples->mFreq;		
		params.mChannels = samples->mChannel;
		params.mSampleNum = samples->mSampleNum;

		static FormatMap m[] = {
			{AV_SAMPLE_FMT_S16,AUDIO_S16SYS},
			{AV_SAMPLE_FMT_S32,AUDIO_S32SYS},
			{AV_SAMPLE_FMT_FLT,AUDIO_F32SYS },
			{ AV_SAMPLE_FMT_S32P,AUDIO_F32SYS },
			{ AV_SAMPLE_FMT_FLTP,AUDIO_F32SYS },
			
		};

		params.mFormat = 0;
		for (uint32_t i = 0; i < sizeof(m) / sizeof(m[0]); i++) {
			if (samples->mFormat == m[i].ffmpegFormat) {
				params.mFormat = m[i].sdlFormat;
				break;
			}
		}
		
		return params.mFormat != 0;
	}
	//
	//  打开关闭音频设备
	//
	bool SDL2AudioDevice::open(const Param& params) {
		if (mIsOpened) {
			return false;
		}

		if (params.mFormat == 0) {
			return false;
		}

		SDL_AudioSpec wantedSpec = {};
		wantedSpec.freq = params.mFreq;
		wantedSpec.format = params.mFormat;
		wantedSpec.channels = params.mChannels;
		wantedSpec.silence = 0;
		wantedSpec.samples = FFL_MAX(params.mSampleNum,1024);
		wantedSpec.callback = SDL2AudioDevice::fillAudio;
		wantedSpec.userdata = this;	

		SDL_AudioSpec  obtained = {};
		if (SDL_OpenAudio(&wantedSpec, &obtained) < 0) {
			FFL_LOG_WARNING("SDL_OpenAudio fail");
			return false;
		}

		mDeviceParam.mFreq = obtained.freq;
		mDeviceParam.mFormat = obtained.format;
		mDeviceParam.mChannels = obtained.channels;
		mDeviceParam.mSampleNum = params.mSampleNum;


		uint32_t perSampleSize = SDL_AUDIO_BITSIZE(mDeviceParam.mFormat) / 8 * mDeviceParam.mChannels;
		uint32_t bufSize = (params.mSampleNum * perSampleSize *5);
		bufSize = FFL_MAX(4096 * 5,bufSize);
		FFL_SafeFree(mByteStream);
		mBytesPerSec = mDeviceParam.mFreq * perSampleSize;
		char* buf = new char[bufSize];
		mByteStream = new FFL::ByteStream();
		mByteStream->setData((uint8_t*)buf, 0, bufSize);
		SDL_PauseAudio(0);	
		mIsOpened = true;
		return true;
	}

	void SDL2AudioDevice::close() {
		if (mIsOpened) {			
			SDL_PauseAudio(1);
			SDL_CloseAudio();
			mIsOpened = false;
			mCond.signal();
		}
	}
	//
	//  写一帧数据
	//	
	bool SDL2AudioDevice::writeFrame(FFLSample* samples) {
		if (!mIsOpened) {
			Param params = {};
			open(params);
		}

		FFL::CMutex::Autolock l(mLock);
		if (mByteStream) {
			while (mIsOpened && mByteStream->getCapacity() - mByteStream->getSize() < (uint32_t)samples->mLinesize){
				FFL_LOG_DEBUG_TAG(TAG_AUDIO,"audio wait..");
			    mCond.wait(mLock);
			}

			if (!mIsOpened) {
				return false;
			}

			FFL_LOG_DEBUG_TAG(TAG_AUDIO,"audio writeBytes..");
			mByteStream->writeBytes((const int8_t*)samples->mData[0], samples->mLinesize);
		}
		return true;
	}	

	void  SDL2AudioDevice::fillAudio(void *userdata, Uint8 *stream, int len) {
		SDL2AudioDevice* pThis = (SDL2AudioDevice*)userdata;
		if (stream && len) {
			pThis->write(stream, len);
		}
	}
	
	void SDL2AudioDevice::write(uint8_t *stream, uint32_t len) {
		SDL_memset(stream, 0, len);

		FFL_LOG_DEBUG_TAG(TAG_AUDIO,"audio read..len=%d",len);
		FFL::CMutex::Autolock l(mLock);
		if (mByteStream == NULL || mByteStream->getSize() == 0) {
			mCond.signal();
			return;
		}

		uint32_t copyLen = 0;
		if (len > mByteStream->getSize()) {
			copyLen = mByteStream->getSize();
		}
		else {
			copyLen = len;
		}

		FFL_LOG_DEBUG_TAG(TAG_AUDIO, "audio read..copylen=%d", copyLen);
		if (copyLen > 0) {
			if (g_TempBufferLen < copyLen) {
				FFL_SafeFreeA(g_TempBuffer);
				g_TempBuffer = new int8_t[copyLen];
				g_TempBufferLen = copyLen;
			}

			FFL_LOG_WARNING("SDL2Module::writeAudio1  %d", copyLen);
			mByteStream->readBytes((int8_t*)g_TempBuffer, copyLen);
			FFL_LOG_WARNING("SDL2Module::writeAudio2");
			SDL_MixAudio(stream, (Uint8*)g_TempBuffer, len, SDL_MIX_MAXVOLUME);
		}else {
			//FFL_LOG_CRIT("autdio read..copylen=%d", copyLen);
		}
		mCond.signal();
	}
	//
	//  获取播放设备硬件的延迟
	//
	int64_t SDL2AudioDevice::getDeviceDelayUs() const{
		return 0;
	}
	//
	// 获取缓冲的延迟时间
	//
	int64_t SDL2AudioDevice::getCacheDelayUs() {
		if (mByteStream == NULL) {
			return 0;
		}

		int64_t bytes = 0;
		{
		
			FFL::CMutex::Autolock l(&mLock);
			bytes=mByteStream->getSize();
		} 
		//bytes += 4096;
		
		return (int64_t)((double)(bytes *1000 * 1000) / mBytesPerSec);		
	}

	int64_t SDL2AudioDevice::getCacheDelay(FFL::TimeBase& base) {
		if (mByteStream == NULL) {
			return 0;
		}

		int64_t bytes = 0;
		{

			FFL::CMutex::Autolock l(&mLock);
			bytes = mByteStream->getSize();
		}

		return (int64_t)((double)(bytes *  base.mDen) / (mBytesPerSec * base.mNum));
	}

	int64_t SDL2AudioDevice::getCacheBytes() {
		if (mByteStream == NULL) {
			return 0;
		}

		int64_t bytes = 0;
		{

			FFL::CMutex::Autolock l(&mLock);
			bytes = mByteStream->getSize();
		}
		return bytes;
	}
}
