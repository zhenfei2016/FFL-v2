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
void myLog(int level, const char *format, va_list v) 
{
	char str[1024] = "";
	vsnprintf(str, 1024 - 1, format, v);


	::OutputDebugStringA(str);
	::OutputDebugStringA("\r\n");
}
#endif
extern int playerMain();
int main() {

	FFL::startMemoryWatch();
	
    FFL_LogSetLevel(FFL_LOG_LEVEL_CRIT);	
	//FFL_LogSetCallback(myLog);

    //FFL_LogSetLevel(FFL_LOG_LEVEL_ALL);
    FFL_LOG_INFO("start player");
	playerMain();
	FFL_LOG_INFO("quit player");

	FFL_malloc_memory_dump();
	getchar();
	return 0;
}
