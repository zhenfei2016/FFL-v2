/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegAudioDecoder.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/15
*
*  音频解码器
*
*/

#include "MessageFFMpegPacket.hpp"
#include "NodeFFMpegAudioDecoder.hpp"
#include "FFMpegStream.hpp"
#include "FFL_Player.hpp"
#include "FFL_Texture.hpp"

namespace player {
	NodeFFMpegAudioDecoder::NodeFFMpegAudioDecoder(FFMpegStream* stream):
		NodeFFMpegDecoder(stream){
		mStream = stream;
		FFL_ASSERT(stream);
		setName("NodeFFMpegAudioDecoder");		
		mMessageCache = new FFL::PipelineMessageCache(MSG_FFMPEG_AUDIO_FRAME);
		mLatelyPts = 0;		
	}
	NodeFFMpegAudioDecoder::~NodeFFMpegAudioDecoder()
	{
	}
	
	void NodeFFMpegAudioDecoder::handleDecodedFrame(AVFrame* frame)
	{
		double speed = 1.0f;
		message::FFMpegAudioFrame* texture = 0;
		FFL::sp<FFL::PipelineMessage> msg = message::createMessageFromCache(mMessageCache, &texture, MSG_FFMPEG_AUDIO_FRAME);
		texture->fillAvframe(frame);
		texture->mSamples.mStreamId = mStream->getIndex();
		texture->mSamples.mFreq = mStream->getAudioSamples();
		correctTimestamp(texture);

		if (FFL_OK != postMessage(mFrameOutput.mId, msg)) {
			msg->consume(this);
		}	
	}
	void NodeFFMpegAudioDecoder::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {
		postMessage(mFrameOutput.mId, eof);
	}
	//
	//  修正时间戳
	//
	void NodeFFMpegAudioDecoder::correctTimestamp(message::FFMpegAudioFrame* sample)
	{
		AVFrame* frame = sample->mFrame;
		//
		//  计算pts
		//
		int64_t pts = frame->pts;
		if (pts == AV_NOPTS_VALUE) {
			pts = frame->pkt_dts;
		}

		sample->mSamples.mOrginalPts = pts;
		frame->pts = pts;	
	}
}
