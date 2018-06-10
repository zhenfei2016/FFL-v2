/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegVideoDecoder.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/24
*
*  视频解码
*
*/

#pragma once
#include "NodeFFMpegDecoder.hpp"

namespace player {
	class FFMpegStream;
	class NodeFFMpegVideoDecoder : public NodeFFMpegDecoder {
	public:
		NodeFFMpegVideoDecoder(FFMpegStream* stream);
		~NodeFFMpegVideoDecoder();

		void correctTimestamp(message::FFMpegVideoFrame* texture);
	private:
		//
		//  解码出来的图片 ,frame通过av_frame_alloc申请出来的
		//  使用完这个frame后，会自动的 
		//  av_frame_unref(frame);
		//  av_frame_free(&frame);
		//
		void handleDecodedFrame(AVFrame* frame);
		//
		//  接收到eof消息
		//
		void handleEOF(const FFL::sp<FFL::PipelineMessage>& eof);
	public:

		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
	};

}