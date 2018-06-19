/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  MainPlayer.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/12
*
*  播放器逻辑
*
*/

#include "Player.hpp"
#include "SDL2Module.hpp"
#include <utils/FFL_MemoryWatch.hpp>
#if WIN32
#include <windows.h>
#endif


bool keyPressed(void* userdata, int key) {
	player::FFLPlayer* player=(player::FFLPlayer*) userdata;

	switch (key)
	{
	case 's':
	case 'S':
		//
		//  开始播放
		//
		player->play(player->mUrl.c_str());
		break;
	case 'q':
	case 'Q':
        //
		// 停止播放
		//
		player->stop();
		return false;
	case '.':
	case '>':
	{   
	    //
		//  加速
		//
		uint32_t speed = player->getSpeed();
		speed = FFL_MIN(speed + 5, 300);
		player->setSpeed(speed);
	}
		break;

	case ',':
	case '<':
	{		
		//
		//  减速
		//
		uint32_t speed = player->getSpeed();
		speed =FFL_MAX(speed- 5,10);
		player->setSpeed(speed);
	}
		
		break;
	case '1':
		//
		//  前跳一点
		//
	{
        int64_t cur=player->getPositionUs();
		cur += 1000 * 1000;
		if (cur > player->getDurationUs()) {
			cur = player->getDurationUs()-10;
		}
        player->setPositionUs(cur);
	}
		break;
	case '2':
		//
		//  后跳一点
		//
	{
        int64_t cur=player->getPositionUs();
		cur -= 1000 * 1000;
		if (cur < 0) {
			cur = 0;
		}
		player->setPositionUs(cur);
	}
	break;
	default:
		break;
	}
	
	return true;
}

int playerMain() {
	player::FFLPlayer player;	
	std::string url;	
#if WIN32
	url = "d://movic//sintel.ts";       
	//url = "d://movic//Nocturne.m4a";
	//url = "d://movic//test.avi";
	url = "d://movic//zhuoyaoji.mp4";
	
	
#else
	url = "/Users/zhufeifei/work/testvideo/sintel.ts";
#endif
	player.mUrl = url;
	//player.play(url.c_str());
	player::SDL2Loop(keyPressed,&player);
	player.release();			
	return 0;
}
