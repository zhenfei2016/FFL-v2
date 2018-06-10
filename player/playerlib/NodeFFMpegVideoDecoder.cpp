/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegVideoDecoder.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/24
*
*  视频解码
*
*/

#include "MessageFFMpegPacket.hpp"
#include "NodeFFMpegVideoDecoder.hpp"
#include "FFMpegStream.hpp"
#include "FFL_Player.hpp"
#include "FFL_Texture.hpp"

namespace player {
	NodeFFMpegVideoDecoder::NodeFFMpegVideoDecoder(FFMpegStream* stream) :
		 NodeFFMpegDecoder(stream){
		setName("NodeFFMpegVideoDecoder");		
		mMessageCache = new FFL::PipelineMessageCache(MSG_FFMPEG_VIDEO_FRAME);		
	}
	NodeFFMpegVideoDecoder::~NodeFFMpegVideoDecoder()
	{
	}

	void NodeFFMpegVideoDecoder::handleDecodedFrame(AVFrame* frame)
	{
		double speed = 1.0f;
		message::FFMpegVideoFrame* texture = 0;
		FFL::sp<FFL::PipelineMessage> msg = message::createMessageFromCache(mMessageCache, &texture, MSG_FFMPEG_VIDEO_FRAME);
		texture->fillAvframe(frame);
		texture->mTexture.mStreamId = mStream->getIndex();
		correctTimestamp(texture);

		if (FFL_OK != postMessage(mFrameOutput.mId, msg)) {
			msg->consume(this);
		}	
	}
	void NodeFFMpegVideoDecoder::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof)
	{
		postMessage(mFrameOutput.mId, eof);
	}


	//
	//  修正时间戳
	//
	void NodeFFMpegVideoDecoder::correctTimestamp(message::FFMpegVideoFrame* texture)
	{
		AVFrame* frame = texture->mFrame;
		//
		//  计算pts
		//
		int64_t pts = frame->pts;
		if (pts == AV_NOPTS_VALUE) {
			pts = frame->pkt_dts;
		}
		
		texture->mTexture.mOrginalPts = pts;
		frame->pts = pts;		
	}
}
