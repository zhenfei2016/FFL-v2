/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegDecoder.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  ����������
*
*/

#include "MessageFFMpegPacket.hpp"
#include "NodeFFMpegDecoder.hpp"
#include "FFMpegStream.hpp"
#include "FFL_Player.hpp"
#include "FFL_Texture.hpp"

namespace player {
	NodeFFMpegDecoder::NodeFFMpegDecoder(FFMpegStream* stream) {
		mStream = stream;
		FFL_ASSERT(stream);
	}
	NodeFFMpegDecoder::~NodeFFMpegDecoder()
	{
	}

	//
	// ������յ�����Ϣ��
	//
	bool NodeFFMpegDecoder::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata) {
		bool ret = false;
		switch (msg->getType())
		{
		case MSG_FFMPEG_AVPACKET:			
			ret= decodeMessageFFMpegPacket((message::FFMpegPacket*) (msg->getPayload()));
            msg->consume(this);
			break;
		case MSG_CONTROL_READER_EOF:
			decode(NULL);
			handleEOF(msg);
			ret = true;
			break;
		default:
            msg->consume(this);
			break;
		}

		
		
		
		return ret;
	}	
	//
	//  ����һ��Ϣ
	//
	bool NodeFFMpegDecoder::decodeMessageFFMpegPacket(message::FFMpegPacket* msg) {
		//
		//  ��������
		//
		if (openDecoder(msg))
		{
			return decode(msg->mPacket);
		}
		return false;
	}
	//
	//  �򿪽�����
	//
	bool NodeFFMpegDecoder::openDecoder(message::FFMpegPacket* msg)
	{		
		if (mStream) {
			mCodecCtx = mStream->mFFMpegCodecCtx;
		}
		return mCodecCtx != NULL;
	}
	//
	//  �رս�����
	//
	void NodeFFMpegDecoder::closeDecoder() {

	}
	
	bool NodeFFMpegDecoder::decode(AVPacket *pkt)
	{
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
				return false;
			}else if (err < 0){
				FFL_LOG_WARNING("Error during decoding\n");
				return false;
			}

			//
			//  �ɹ������һ֡����
			//
			FFL_LOG_INFO("frame=%d pts=%" lld64 " width=%d,height=%d keyframe=%d",
				mCodecCtx->frame_number, frame->pts,
				frame->width, frame->height, frame->key_frame);
			handleDecodedFrame(frame);			
		}

		return true;
	}

	void NodeFFMpegDecoder::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {

	}
    
    void NodeFFMpegDecoder::handleDiscardCache(const FFL::sp<FFL::PipelineMessage>& eof) {
        
    }
}
