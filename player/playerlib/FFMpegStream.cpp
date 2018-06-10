/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFMpegStream.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/04/16 
 *  
 *  流基本信息
 *
*/
#include "FFMpegStream.hpp"
#include "NodeFFMpegVideoDecoder.hpp"
#include "NodeFFMpegAudioDecoder.hpp"

namespace player {
	FFMpegStream::FFMpegStream() {

	}
	FFMpegStream::~FFMpegStream() {

	}

	void FFMpegStream::getTimebase(FFL::TimeBase& units) {
		units.mDen = mFFMpegStream->time_base.den;
		units.mNum = mFFMpegStream->time_base.num;
	}
	//
	//  获取音频采样率
	//
	uint32_t FFMpegStream::getAudioSamples() {
		return mFFMpegCodecCtx->sample_rate;
	}
	//
	//  创建这个流需要的解码器
	//
	FFL::sp<Decoder> FFMpegStream::createDecoder() {
		if (getStreamType() == STREAM_TYPE_VIDEO) {
			mDecoder=new NodeFFMpegVideoDecoder(this);
		}
		else if (getStreamType() == STREAM_TYPE_AUDIO) {
			mDecoder = new NodeFFMpegAudioDecoder(this);
		}
		return mDecoder;
	}
	

}