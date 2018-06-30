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



//#include <utils/FFL_MemoryWatch.hpp>
#include "SDL2Module.hpp"
#if WIN32
#include <windows.h>
#endif

#include "Player.hpp"

class TestListener : public player::IPlayerListener {
public:
	TestListener(player::FFLPlayer* player):mPlayer(player){

	}

	virtual void onPrepared(int suc) {
		printf("onPrepared suc=%d\n",suc);
		if (suc) {
			mPlayer->start();
		}
	}
	//
	// 视频大小改变了
	// widht:宽度 ，height：高度
	// aspectRatioNum/aspectRatioDen  宽高比
	//
	virtual void onVideoSizeChanged(int32_t width, int32_t height, int32_t aspectRatioNum, int32_t aspectRatioDen) {
		printf("onVideoSizeChanged: width=%d height=%d aspectRatio=%d/%d\n",
			width, height,
			aspectRatioNum,
			aspectRatioDen);	
		//mPlayer->setSurfaceSize(width,height);
	}	
	//
	//  视频播放结束回调
	//
	virtual void onComplete() {
		printf("onComplete:  \n");
	}
	//
	//  seek结束回调
	//
	virtual void onSeekComplete(int64_t pos, int64_t duration) {
		char durationBuf[256] = {};
		FFL_usToString(duration,durationBuf);

		char posBuf[256] = {};
		FFL_usToString(pos, posBuf);
		printf("onSeekComplete: pos=%s duration=%s \n", posBuf, durationBuf);
	}
	//
	// 错误码
	//
	virtual void onError(int32_t errCode, int32_t errVal) {
		printf("onSeekComplete: errCode=%d errVal=%d \n", errCode, errVal);
	}
	//
	// 缓冲中
	//
	virtual void onBufferingStart() {
		printf("onBufferingStart:  \n");
	}
	virtual void onBufferingUpdate() {
		printf("onBufferingUpdate:  \n");
	}
	//
	// 一些其他的消息
	//
	virtual void onMessage(int32_t msg, int32_t parma1, int32_t param2) {
		printf("onMessage: msg=%d param1=%d parma2=%d\n", msg, parma1, param2);
	}

private:
	player::FFLPlayer* mPlayer;
};

bool gIsPause = false;
bool keyPressed(void* userdata, int key) {
	player::FFLPlayer* player=(player::FFLPlayer*) userdata;

	switch (key)
	{
	case 'r':
	case 'R':
		//
		//  开始播放
		//
		player->prepare();
		break;
	case 'q':
	case 'Q':
        //
		// 停止播放
		//
		player->stop();
        FFL_LOG_DEBUG("player stop.");
		return false;
	case '+':
	case '=':
	{
		//
		//  加速
		//
		uint32_t speed = player->getSpeed();
		speed = FFL_MIN(speed + 5, 300);
		player->setSpeed(speed);
		printf("change speed:%d  \n", speed);
		break; 
	}

	case '-':
	case '_':
	{		
		//
		//  减速
		//
		uint32_t speed = player->getSpeed();
		speed =FFL_MAX(speed- 5,10);
		player->setSpeed(speed);
		printf("change speed:%d  \n",speed);
	}
		
		break;
	
	case 'a':
	case 'A':
		//
		//  后跳一点 向后1s
		//
	{
		int64_t cur = player->getCurrentPosition();
		int64_t target = cur - 4000 * 1000;
		if (target < 0) {
			target = 0;
			break;
		}
		player->seekTo(target);
		
		char currentBuf[256] = {};
		FFL_usToString(cur, currentBuf);
		char targetBuf[256] = {};
		FFL_usToString(target, targetBuf);
		printf("seekTo <- %s-%s \n", currentBuf, targetBuf);
	}
	break;
	case 'd':
	case 'D':
		//
		//前跳一点 向前1s
		//
	{
		int64_t cur = player->getCurrentPosition();
		int64_t target = cur + 4000 * 1000 * 60;
		if (target > player->getDuration()) {
			target = player->getDuration() - 10;
		}
		player->seekTo(target);

		char currentBuf[256] = {};
		FFL_usToString(cur, currentBuf);
		char targetBuf[256] = {};
		FFL_usToString(target, targetBuf);
		printf("seekTo -> %s-%s \n", currentBuf, targetBuf);
	}
	break;
	case 'w':
	case 'W':
	{   //
		//  音量增加
		//
		int32_t volume;
		player->getVolume(volume);		
		volume += 10;
		volume=FFL_MIN(255,volume);
		player->setVolume(volume);

	
		printf("volume=%d \n", volume);
	}
		break;
	case 's':
	case 'S':
	{   //
		//  音量减少
		//
		int32_t volume;
		player->getVolume(volume);
		volume -= 10;
		volume = FFL_MAX(0, volume);
		player->setVolume(volume);


		printf("volume=%d \n", volume);
	}
	break;
	case 'p':
	case 'P':		
		player->pause(gIsPause?0:1);
		gIsPause = !gIsPause;
		break;
	default:
		break;
	}
	
	return true;
}

int playerMain() {
	player::FFLPlayer player;
	TestListener listener(&player);
	player.setListener(&listener);

	std::string url;	
#if WIN32
	url = "d://movic//sintel.ts";       
	//url = "d://movic//Nocturne.m4a";
	//url = "d://movic//test.avi";
	//url = "d://movic//zhuoyaoji.mp4";	
	
#else
	url = "/Users/zhufeifei/work/testvideo/sintel.ts";
#endif
	
	player.setUrl(url.c_str());
    //player.prepare();
    player::SDL2Loop(keyPressed,&player);
	return 0;
}
