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
		//  已经处理完成了，可以回收了		
		//
		virtual void consume();
	public:
		AVPacket* mPacket;

		uint32_t mStreamIndex;
		//
		// 这一帧微妙
		//
		uint64_t mDurationUs;

		uint64_t mPtsUs;
	};

}
