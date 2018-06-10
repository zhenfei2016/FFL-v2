/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  MessageFFMpegPacket.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/10 
 *  
 *  Î´½âÂë°ü
 *
*/
#include "MessageFFMpegPacket.hpp"

namespace message {
	FFMpegPacket::FFMpegPacket()
	{
        setType(MSG_FFMPEG_AVPACKET);
        mPacket=av_packet_alloc();
    }
	FFMpegPacket::~FFMpegPacket()
	{
        av_packet_free(&mPacket);
    }
    

}
