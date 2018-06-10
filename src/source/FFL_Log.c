/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Log
*  Created by zhufeifei(34008081@qq.com) on 2017/7/12
*  打印日志帮助类
*
*/
#include <stdio.h>
#include <stdarg.h>

#include "FFL_Log.h"
#include "FFL_Time.h"
#include "thread/FFL_Thread.h"


static void FFL_print_log(int level, const char *format, va_list vl);

/* 
  每条的输出日志日志最长

  */
#define MAX_LOG_LENGTH	2048

#ifdef _DEBUG
static FFL_LogLevel s_loglevel = FFL_LOG_LEVEL_ALL;
#else
static FFL_LogLevel s_loglevel = FFL_LOG_LEVEL_ERROR;
#endif 

static FILE *s_out_fd;
static FFL_LogHookFun s_print_log= FFL_print_log;

static void internalPrintLog(int level, const char *format, va_list args) 
{
	if (level > s_loglevel)
		return;
	s_print_log(level, format, args);
}
static const char *s_levels_string[] = {
  "CRI",
  "ERR",
  "WAR",
  "INF",
  "DBG"
};

void FFL_LogSetLevel(FFL_LogLevel level)
{
	s_loglevel = level;
}
FFL_LogLevel FFL_LogGetLevel()
{
	return s_loglevel;
}

void FFL_LogSetOutput(FILE *file)
{
	s_out_fd = file;
}

void FFL_LogSetCallback(FFL_LogHookFun cbp)
{
	s_print_log = cbp;
}

static void FFL_print_log(int level, const char *format, va_list vl)
{
	char str[MAX_LOG_LENGTH]="";		
	char timeFormat[64] = { 0 };
	if (level>s_loglevel)
		return;

	vsnprintf(str, MAX_LOG_LENGTH - 1, format, vl);
	if ( !s_out_fd) 
		s_out_fd = stdout;
	
	{
		FFL_getNowString(timeFormat);
#ifdef WIN32
		fprintf(s_out_fd, "%s:%s tid=%d %s \n", s_levels_string[level], timeFormat, (int)FFL_CurrentThreadID(),  str);	
#else
        fprintf(s_out_fd, "%s:t=%" lld64 " tid=%d %s \n", s_levels_string[level], FFL_getNowUs(), (int)FFL_CurrentThreadID(),  str);
#endif
#ifdef _DEBUG
		fflush(s_out_fd);
#endif
	}
}

void FFL_log_cri(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_CRIT, format, args);
	va_end(args);
}

void FFL_log_error(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_ERROR, format, args);
	va_end(args);
}
void FFL_log_waring(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_WARNING, format, args);
	va_end(args);
}
void FFL_log_info(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_INFO, format, args);
	va_end(args);
}
void FFL_log_debug(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}

void FFL_log_cri_tag(const char *tag,const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_CRIT, format, args);
	va_end(args);
}
void FFL_log_error_tag(const char *tag,const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_ERROR, format, args);
	va_end(args);
}
void FFL_log_waring_tag(const char *tag,const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_WARNING, format, args);
	va_end(args);
}
void FFL_log_info_tag(const char *tag,const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_INFO, format, args);
	va_end(args);
}
void FFL_log_debug_tag(const char *tag,const char *format, ...)
{
	va_list args;
	va_start(args, format);
	internalPrintLog(FFL_LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}
