/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFMpeg.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/10 
 *  
 *
 *
*/
#pragma once

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
	#include <libavdevice/avdevice.h>
	#include <libavutil/imgutils.h>
}




//
//  ����ת��Ϊffmpeg��seekfile��λ�µ�λ��
//
int64_t FFMPegUsToSeekfileTimestamp(int64_t us);