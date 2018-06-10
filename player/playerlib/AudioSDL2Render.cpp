/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioSDL2Render.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  声音渲染类 sdl
*
*/

#include "AudioSDL2Render.hpp"
#include "AudioResample.hpp"
#include "MessageFFMpegFrame.hpp"
#include "FFL_Player.hpp"
#include "SDL2Module.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>
#include "FFL_PlayerEvent.hpp"
#include "Statistic.hpp"
#include "TimestampExtrapolator.hpp"
namespace player {
	AudioSDL2Render::AudioSDL2Render(FFL::sp<SDL2Module> sdl) {
		setName("AudioSDL2Render");
		mSDL2 = sdl;
		mResample = new AudioResample();
		mFrameIndex = -1;
	}
	AudioSDL2Render::~AudioSDL2Render() {
		mSDL2 = NULL;
		FFL_SafeFree(mResample);
	}	

	//
	//  停止render
	//
	void AudioSDL2Render::stop() {
		mSDL2->getAudioDevice()->close();
	}

	class CacheSizeCalc : public FFL::PipelineAsyncConnectorFixSize::MessageSizeCalculator {
	public:
		//
		//  这个仅仅计算消息列表的大小，不要加一些复杂的过程操作
		//
		virtual  uint32_t calcSize(std::list< FFL::sp<FFL::PipelineMessage> >& msgList) {

			uint32_t duration = 0;
			if (msgList.size() <= 1) {			
				return 0;
			}

			std::list< FFL::sp<FFL::PipelineMessage> >::iterator it=msgList.begin() ; 
			int64_t firstPts = 0;
			{
				FFL::sp<FFL::PipelineMessage> msg1 = msgList.back();
				if (msg1->getType() != MSG_FFMPEG_AUDIO_FRAME) {
					return 0;
				}
				FFL::PipelineMessagePayload* payload = msg1->getPayload();
				message::FFMpegAudioFrame* frame = (message::FFMpegAudioFrame*)(payload);
				firstPts=frame->mSamples.mOrginalPts;
			}

			int64_t lastPts = 0;
			{
				FFL::sp<FFL::PipelineMessage> msg1 = msgList.front();
				if (msg1->getType() != MSG_FFMPEG_AUDIO_FRAME) {
					return 0;
				}

				FFL::PipelineMessagePayload* payload = msg1->getPayload();
				message::FFMpegAudioFrame* frame = (message::FFMpegAudioFrame*)(payload);
				lastPts = frame->mSamples.mOrginalPts;
			}				

			uint32_t us= (uint32_t)(((double)(lastPts - firstPts) * 1000000) / 90000);
			return us;
		}
	};
	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > AudioSDL2Render::onCreateConnector(const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		//
		//  存储500ms的数据
		//
		uint32_t maxUs=1000*1000*500;
		FFL::PipelineAsyncConnectorFixSize* conn= new FFL::PipelineAsyncConnectorFixSize(maxUs);
		conn->setMessageSizeCalculator(new CacheSizeCalc());
		return conn;

		//return new FFL::PipelineAsyncConnector();
	}

	bool AudioSDL2Render::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata) {
		if (!AudioRender::handleReceivedData(msg, userdata)) {
			if (msg->getType() == MSG_CONTROL_READER_EOF) {
				mSDL2->getAudioDevice()->close();
				event::postPlayerEvent(this, event::EVENT_AUDIO_RENDER_LAST_FRAME);
			}	
			return false;
		}
		return true;
	}

	static bool gFirst = true;
	static int64_t gOrginalPts = 0;
	//
	//  收到待显示的纹理包
	//	
	void AudioSDL2Render::onShowSamples(FFLSample* samples)
	{
		int64_t t1 = FFL_getNowUs();		
		FFLSample targetSample = {};
		targetSample.mChannel = 2;
		targetSample.mChannelLayout = AV_CH_LAYOUT_STEREO;
		targetSample.mFormat = AV_SAMPLE_FMT_S16;
		targetSample.mFreq = samples->mFreq;

		if (samples->mChannelLayout <= 0) {
			if(samples->mChannel==1)
			   samples->mChannelLayout = AV_CH_LAYOUT_MONO;
			else if (samples->mChannel == 2)
				samples->mChannelLayout = AV_CH_LAYOUT_STEREO;
			else
				samples->mChannelLayout = AV_CH_LAYOUT_MONO;
		}
		
		mResample->resample(samples, &targetSample, getOwner()->mClock->speed());
		SDL2AudioDevice* audioDevice = mSDL2->getAudioDevice();
		if (audioDevice->isClosed()) {
			SDL2AudioDevice::Param params = {};
			if (audioDevice->coverParams(&targetSample, params)) {
				audioDevice->open(params);
			}else {
				FFL_LOG_WARNING("Failed to open SDL audio");
			}
		}
		
		int64_t pts = 0;
		int64_t delay;
		if(1)
		{   //
			// 当前渲染到那地方了
			//
			FFL::TimeBase units = {};
			getOwner()->getStream(samples->mStreamId)->getTimebase(units);
			delay = audioDevice->getCacheDelay(units) ;
			pts = samples->mOrginalPts - delay;
			gOrginalPts = samples->mOrginalPts;			
			mStatistic->renderAudioFrame(pts, FFL_getNowUs());
		}

		if (gFirst ) {
			//
			// 更新主时钟
			//
			FFL::sp<Stream> stream = getOwner()->getStream(samples->mStreamId);
			FFL::TimeBase tb;
			stream->getTimebase(tb);
			mTimestampExtrapolator->update(pts,tb);
		}
		//
		//   pts - 
		//
		mSDL2->addSample(&targetSample);
		
		av_freep(targetSample.mData);

		mStatistic->renderAudioDelayUs(audioDevice->getDeviceDelayUs());
		if (mFrameIndex == -1) {
			mFrameIndex = 0;
			event::postPlayerEvent(this, event::EVENT_AUDIO_RENDER_FIRST_FRAME);
		}

		if (mFrameIndex >= 8) {
			int i = 0;
			i++;
		}
		mFrameIndex++;
	}
}
