/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioResample.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  声音的重采样
*
*/

#include "AudioResample.hpp"
#include "FFMpeg.hpp"

namespace player {
	AudioResample::AudioResample():mSwrCtx(NULL){
	}

	AudioResample::~AudioResample() {
		if (mSwrCtx) {
			swr_free(&mSwrCtx);
		}
	}
	
	bool AudioResample::createSwr(const FFLSample* in, FFLSample* out,uint32_t spped) {

		if (mSwrCtx && (mSrc.mChannelLayout != in->mChannelLayout ||
			mSrc.mFormat != in->mFormat ||
			mSrc.mFreq != in->mFreq ||
			mSrc.mSpeed != spped ||
			mDst.mFormat != out->mFormat ||
			mDst.mFreq != out->mFreq)) {
			swr_free(&mSwrCtx);
		}

		if (!mSwrCtx) {
			mSwrCtx = swr_alloc();

			mSrc.mChannelLayout = in->mChannelLayout;
			mSrc.mFreq = in->mFreq;
			mSrc.mFormat = in->mFormat;
			mSrc.mSpeed = spped;

			//
			//  输入数据的声道布局，采样率，采样格式
			//
			av_opt_set_int(mSwrCtx, "in_channel_layout", mSrc.mChannelLayout, 0);
			av_opt_set_int(mSwrCtx, "in_sample_rate", mSrc.mFreq, 0);
			av_opt_set_sample_fmt(mSwrCtx, "in_sample_fmt", (AVSampleFormat)mSrc.mFormat, 0);

			//
			//  输出数据的声道布局，采样率，采样格式
			//	
			mDst.mChannelLayout = out->mChannelLayout;
			mDst.mFreq = out->mFreq;
			mDst.mFormat = out->mFormat;

			av_opt_set_int(mSwrCtx, "out_channel_layout", mDst.mChannelLayout, 0);
			av_opt_set_int(mSwrCtx, "out_sample_rate", mDst.mFreq, 0);
			av_opt_set_sample_fmt(mSwrCtx, "out_sample_fmt", (AVSampleFormat)mDst.mFormat, 0);		
			if (swr_init(mSwrCtx) < 0) {
				FFL_LOG_WARNING("AudioResample::initSwr init fail");
				swr_free(&mSwrCtx);
			}
		}	
		return true;
	}
	void AudioResample::destroySwr() {
	}

	bool AudioResample::resample(const FFLSample* in, FFLSample* out,uint32_t speed) {		
		if (!createSwr(in, out, speed)) {
			return false;
		}

		int64_t delay=swr_get_delay(mSwrCtx, in->mFreq);
		out->mSampleNum =(uint32_t) av_rescale_rnd(
			delay + in->mSampleNum,
			out->mFreq, in->mFreq, AV_ROUND_UP) ;


		if (speed != 100) {
			int32_t diffSampleNum =0;
			if (speed < 100) {
				diffSampleNum= (int32_t)((double)(out->mSampleNum * (100 - speed)) / 100.f);
			}else {
				diffSampleNum = (int32_t)((double)(out->mSampleNum * (speed - 100)) / 100.f);
				diffSampleNum = -diffSampleNum;
			}
			out->mSampleNum += diffSampleNum;
			if (diffSampleNum!=0) {
				if (swr_set_compensation(mSwrCtx, diffSampleNum, out->mSampleNum) < 0) {
					FFL_LOG_WARNING("swr_set_compensation() failed");
				}
			}
		}


		out->mChannel = av_get_channel_layout_nb_channels(out->mChannelLayout);
		av_samples_alloc_array_and_samples(
			(uint8_t***)&(out->mData), &(out->mLinesize),
			out->mChannel,
			out->mSampleNum ,
			(AVSampleFormat)out->mFormat, 0);

		int ret = swr_convert(mSwrCtx, out->mData, out->mSampleNum,
			(const uint8_t**)in->mData, in->mSampleNum);
		if (ret < 0) {
			FFL_LOG_WARNING("resample Error while converting\n");			
			return false;
		}

        av_samples_get_buffer_size(&(out->mLinesize), out->mChannel,
			ret, (AVSampleFormat)out->mFormat, 0);				
		return true;	
	}
}
