/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFMpegStream.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/16
*
*  流基本信息
*
*/

#pragma once

#include "FFMpeg.hpp"
#include "Stream.hpp"

namespace player {

	class FFMpegStream : public Stream {
	 public:
		FFMpegStream();
		~FFMpegStream();

		//
		//  获取这个流的时间单位，pts,dts以这个为基准
		//
		void getTimebase(FFL::TimeBase& units);
		//
		//  获取音频流音频采样率
		//
		uint32_t getAudioSamples();
		//
		//  创建这个流需要的解码器
		//
		FFL::sp<Decoder> createDecoder();
	public:
		AVCodecContext* mFFMpegCodecCtx;
		AVStream* mFFMpegStream;

	};
}