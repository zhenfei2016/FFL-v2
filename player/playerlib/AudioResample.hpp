/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioResample.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  �������ز���
*
*/
#ifndef _AUDIO_RESAMPLE_HPP_
#define _AUDIO_RESAMPLE_HPP_

#include "FFL_Sample.hpp"
#include "FFMpeg.hpp"

namespace player {
	class AudioResample  {
		struct ResampleFormat
		{
			ResampleFormat() {
				mFormat = 0;
				mChannelNum = 0;
				mChannelLayout = 0;
				mSampleNum = 0;
				mFreq = 0;
				mSpeed = 100;
			}
			uint32_t mFormat;
			uint32_t mChannelNum;
			uint32_t mChannelLayout;
			uint32_t mSampleNum;
			uint32_t mFreq;
			uint32_t mSpeed;
		};
	public:
		AudioResample();
		~AudioResample();
	protected:
		bool createSwr(const FFLSample* in, FFLSample* out, uint32_t speed);
		void destroySwr();
	public:
		//
		//  speed  :��ǰ��Ƶ�Ĳ����ٶ�100�����ٶ�
		//
		bool resample(const FFLSample* in , FFLSample* out,uint32_t speed);
	protected:
		//
		//  ffmpeg���ز�����
		//
		SwrContext *mSwrCtx;		
		ResampleFormat mSrc;
		ResampleFormat mDst;
	};
}

#endif