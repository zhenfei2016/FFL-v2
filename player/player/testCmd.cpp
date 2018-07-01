/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  testCmd.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/20 
*  https://github.com/zhenfei2016/FFL-v2.git
*  命令行测试
*
*/
#include <stdio.h>
#include <FFL_Commandline.h>
#include "Player.hpp"

void play(const char* args,void* userdata) {
	printf("play: %s \n",args? args:"null");
}

void pause(const char* args, void* userdata) {
	printf("pause: %s \n", args ? args : "null");
}

void release(const char* args, void* userdata) {
	printf("release: %s \n", args ? args : "null");
}
static CmdOption  gCmdOption[] = {
	{"play",play,"play video" },
	{ "pause",pause,"pause video" },
	{ "release",release,"release video" },
	{0,0,0}
};


int mainTestCmd() {
	player::FFLPlayer player;

	char cmd[256] = {};
	while (fgets(cmd + 2, 256 - 3, stdin)) {
		cmd[0] = '-';
		cmd[1] = '-';

		const char *argv[] = {
			"",
			cmd,
		};
		int argc = 2;

		if (FFL_parseCommnadline(argc, argv,
			gCmdOption,
			FFL_ARRAY_ELEMS(gCmdOption),
			&player) < 0) {
			printf("unknown command: %s \n",cmd+2);
		}
	}	

	
	return 0;
}
