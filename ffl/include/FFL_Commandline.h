/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Commandline.h   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/19 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  分析命令行帮助函数
*
*/
#ifndef _FFL_COMMANDLINE_HPP_
#define _FFL_COMMANDLINE_HPP_

#include <FFL.h>
#ifdef  __cplusplus
extern "C" {
#endif

/*
* 命令行参数选项
*/
typedef struct CmdOption {
	const char* mName;
	void(*fun)(const char* value);
	const char* nHelp;
}CmdOption;

/*
*   分析命令行
*   argc：参数个数
*   argv：参数数组
*   opts：支持的命令数组，以null结尾
*   size：opts数组的大少
*   命令行格式  --cmd=12344
*   返回命令在opts中的位置，没找到返回-1
*/
int FFL_parseCommnadline(int argc,const char** argv, CmdOption* opts,int size);

#ifdef  __cplusplus
}
#endif

#endif
