/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegDecoder.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  解码器基类
*
*/

#include "MessageFFMpegPacket.hpp"
#include "FFMpegDecoder.hpp"
#include "FFMpegStream.hpp"
#include "PlayerCore.hpp"
#include "VideoTexture.hpp"

namespace player {
	NodeFFMpegDecoder::NodeFFMpegDecoder( AVCodecContext* ctx) {
		mCodecCtx = ctx;	
		mSerialNumber = 0;
		mWaitIFrame = false;
	}
	NodeFFMpegDecoder::~NodeFFMpegDecoder()
	{
	}
	//
	// 处理接收到的消息，
	//
	bool NodeFFMpegDecoder::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata) {
		bool ret = false;
		switch (msg->getType())
		{
		case MSG_FFMPEG_AVPACKET:			
			ret= decodeMessageFFMpegPacket(msg,(message::FFMpegPacket*) (msg->getPayload()));
            msg->consume(this);
			break;
		case MSG_CONTROL_READER_SEEK:
		{
			mSerialNumber = msg->getParam1();
			int64_t flag = msg->getParam2();
			if ( (flag& 0x01) == 1) {
				clearMessage(mFrameOutput.mId);
			}
			mWaitIFrame = true;
			if (FFL_OK != postMessage(mFrameOutput.mId, msg)) {
				msg->consume(this);
			}
		}
			break;	
		case MSG_CONTROL_READER_EOF:
		{
			mSerialNumber = msg->getParam1();
			int64_t flag = msg->getParam2();
			if ((flag & 0x01) == 1) {
				//
				//  清空这个解码器的所有数据，这个解码器就没有用了，关闭了
				//
				decode(msg,NULL, false);
			}
			mWaitIFrame = false;
			handleEOF(msg);
			ret = true;
			break;
		}
		default:
            msg->consume(this);
			break;
		}
		return ret;
	}	
	//
	//  解码一消息
	//
	bool NodeFFMpegDecoder::decodeMessageFFMpegPacket(const FFL::sp<FFL::PipelineMessage>& msg, message::FFMpegPacket* packet) {
		if (mSerialNumber == 0) {
			//
			// 第一次
			//
			mSerialNumber = packet->mSerialNumber;
		}				

		//
		//  解码数据
		//
		if (mCodecCtx)
		{
			return decode(msg,packet->mPacket,false);
		}else {
			FFL_LOG_WARNING("Failed to NodeFFMpegDecoder::decode streamIndex=%d" , packet->mPacket->stream_index);
		}
		return false;
	}	
	bool NodeFFMpegDecoder::decode(const FFL::sp<FFL::PipelineMessage>& msg, AVPacket *pkt, bool discard){
		if (mCodecCtx == NULL) {
			return false;
		}

		int err = 0;
		err = avcodec_send_packet(mCodecCtx, pkt);
		if (err < 0)
		{
			FFL_LOG_WARNING("Error sending a packet for decoding\n");
			return false;
		}

		while (err >= 0){
			AVFrame* frame = av_frame_alloc();
			err = avcodec_receive_frame(mCodecCtx, frame);
			if (err == AVERROR(EAGAIN) || err == AVERROR_EOF) {
				av_frame_free(&frame);
				return false;
			}else if (err < 0){
				av_frame_free(&frame);
				FFL_LOG_WARNING("Error during decoding\n");
				return false;
			}

			if (mWaitIFrame) {				
				if (frame->key_frame) {
					discard = false;
					mWaitIFrame = false;
				}
				else {
					discard = true;
				}
			}
			//
			//  成功解码出一帧数据
			//
			FFL_LOG_INFO("frame=%d pts=%" lld64 " width=%d,height=%d keyframe=%d discard=%d",
				mCodecCtx->frame_number, frame->pts,
				frame->width, frame->height, frame->key_frame,discard);
			if (!discard) {
				handleDecodedFrame(frame, msg->trackId());
			}else {
				av_frame_unref(frame);
				av_frame_free(&frame);
			}
		}

		return true;
	}

	void NodeFFMpegDecoder::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {

	}  

}
