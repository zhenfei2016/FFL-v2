/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegDecoder.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  解码器基类
*
*/

#pragma once
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineMessage.hpp>
#include <pipeline/FFL_PipelineNodeHandler.hpp>
#include <pipeline/FFL_PipelineMessageCache.hpp>

#include "FFMpeg.hpp"
#include "Decoder.hpp"
#include "MessageFFMpegFrame.hpp"
#include "MessageFFMpegPacket.hpp"

namespace player {
	class FFMpegStream;
	class NodeFFMpegDecoder : public Decoder {
	public:
		NodeFFMpegDecoder(FFMpegStream* stream);
		~NodeFFMpegDecoder();

	protected:
		//
		// 处理接收到的消息，
		//
		bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);
	private:
		//
		//  解码一消息
		//
		bool decodeMessageFFMpegPacket(message::FFMpegPacket* msg);
		//
		//  打开解码器，如果对应的编码器已经打开了，则不管他
		//
		bool openDecoder(message::FFMpegPacket* msg);
		//
		//  关闭解码器
		//
		void closeDecoder();
		//
		//  解码一帧数据
		//
		bool decode(AVPacket *pkt);
	private:
		//
		//  解码出来的图片 ,frame通过av_frame_alloc申请出来的
		//  使用完这个frame后，会自动的 
		//  av_frame_unref(frame);
		//  av_frame_free(&frame);
		//
		virtual void handleDecodedFrame(AVFrame* frame)=0;

		virtual void handleEOF(const FFL::sp<FFL::PipelineMessage>& eof);
        
        virtual void handleDiscardCache(const FFL::sp<FFL::PipelineMessage>& eof);
	public:
		AVCodecContext* mCodecCtx;
		FFMpegStream* mStream;
		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
		//
		//  最近一次的pts值
		//
		int64_t mLatelyPts;
		//
		// timebase;
		//
		int mTimebaseDen;
		int mTimebaseNum;


	};

}
