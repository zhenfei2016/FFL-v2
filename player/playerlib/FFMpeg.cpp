/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFMpeg.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/10 
 *  
 *
 *
*/
#include "FFMpeg.hpp"
#include <FFL.h>

//
//  时间错转化为毫秒
//
int64_t FFMPegTimestampToUs(int64_t tm, AVRational& tb) {	
	if (tb.den == 0 || tm==0) {
		return tm;
	}

	//
	//  时间戳一个刻度对应多少毫秒
	//
	double dus = ((double)(1000 * 1000 * tb.num)) / tb.den;
	return (int64_t)(tm* dus);
}


//
//  时间错转化为毫秒
//
int64_t FFMPegUsToTimestamp(int64_t tm, AVRational& tb) {
	//
	//  时间戳一个刻度对应多少毫秒
	//
	double dus = ((double)(1000 * 1000 * tb.num)) / tb.den;
	if (dus < 0) {
		FFL_LOG_WARNING("FFMPegUSToTimestamp fail");
		return tm;
	}
	return (int64_t)((double)tm/dus);
}