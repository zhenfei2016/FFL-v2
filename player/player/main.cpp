/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  main.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/12 
 *  
 *  
 *
*/
#include <stdio.h>
#include <utils/FFL_MemoryWatch.hpp>
#if WIN32
#include <windows.h>
int printLog(int level,const char* tag, const char *format, va_list v) 
{
	if ( tag) {
		if ( strcmp(tag, "MsgQueue") == 0 ||
			strcmp(tag, "timestamp") == 0 ) {
			char str[1024] = {};
			vsnprintf(str+2, 1024 - 1, format, v);
			str[0] = ' ';
			str[1] = ' ';

			char timeStr[128] = {0};
			FFL_getNowString(timeStr);
			::OutputDebugStringA(timeStr);
			::OutputDebugStringA(str);
			::OutputDebugStringA("\r\n");
			return 1;
		}
	}
	return 1;
}
#endif
extern int playerMain();
extern int mainTestCmd();


int main() {

	FFL::startMemoryWatch();

	//FFL_LogHook(printLog);
    FFL_LogSetLevel(FFL_LOG_LEVEL_ALL);
	//FFL_LogSetLevel(FFL_LOG_LEVEL_WARNING);
	if (0) {
		mainTestCmd();
	}
	else {

		FFL_LOG_INFO("start player");
		playerMain();
		FFL_LOG_INFO("quit player");
	}
	FFL_malloc_memory_dump();
	//getchar();
	return 0;
}
