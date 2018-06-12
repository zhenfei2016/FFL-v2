/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  MessageFFMpegPacket.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/10 
 *  
 *  未解码包
 *
*/
#include "MessageFFMpegPacket.hpp"

namespace message {
	FFMpegPacket::FFMpegPacket()
	{
        setType(MSG_FFMPEG_AVPACKET);
        mPacket=av_packet_alloc();
    }
	FFMpegPacket::~FFMpegPacket(){
        av_packet_free(&mPacket);
    }    
	//
	//  已经处理完成了，可以回收了		
	//
	void FFMpegPacket::consume() {
		av_packet_unref(mPacket);
	}
}
