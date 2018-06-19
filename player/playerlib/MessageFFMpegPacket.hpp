/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  MessageFFMpegPacket.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  未解码包
*
*/

#pragma once
#include "MessageBase.hpp"
#include "FFMpeg.hpp"

namespace message {
	//
	//   未解码的的数据包
	//
	class FFMpegPacket : public MessageBase
	{
	public:
		FFMpegPacket();
		~FFMpegPacket();		

		//
		//  是否i桢
		//
		virtual bool isIFrame() const;
		//
		//  是否视频，音频桢
		//
		virtual bool isVideo() const;
		virtual bool isAudio() const;
		//
		//  已经处理完成了，可以回收了		
		//
		virtual void consume();
	public:
		AVPacket* mPacket;
		//
		// 流索引
		//
		uint32_t mStreamIndex;
		//
		// 音频，视频
		//
		uint32_t mPacketType;
		//
		//  是否i桢
		//
		int32_t mIFrame;
		//
		// 
		//
		int32_t mSerialNumber;
	};

}
