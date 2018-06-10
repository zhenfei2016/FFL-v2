/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Time.c
*  Created by zhufeifei(34008081@qq.com) on 2018/06/10
*  时间获取相关的封装函数
*
*/

#include "FFL_Time.h"

#if defined( WIN32)
#include <windows.h>
#include "window_time.c"
#else
#include "linux_time.c"
#endif

void FFL_sleep(int ms)
{	
#if WIN32
	Sleep(ms);
#elif defined(MACOSX) || defined(ANDROID)
	int err;
	struct timespec elapsed, tv;
	elapsed.tv_sec = ms / 1000;
	elapsed.tv_nsec = (ms % 1000) * 1000000;
	do
	{
		tv.tv_sec = elapsed.tv_sec;
		tv.tv_nsec = elapsed.tv_nsec;
		err = nanosleep(&tv, &elapsed);
	} while (err);
#else

#endif
}

int64_t FFL_getNowUs() {
	int64_t nowUs=0;
#if WIN32	
	nowUs = (int64_t)internalGetUs();
#else
	nowUs = internalGetUs();
#endif
	return nowUs;
}

int32_t FFL_getNowMs() {
	int32_t now=(int32_t)(FFL_getNowUs()/1000);
	return now;
}


void FFL_getNowString(char* s) {
	internalGetTimeString(s);
}