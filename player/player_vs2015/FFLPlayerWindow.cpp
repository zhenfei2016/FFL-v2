
#include "FFLPlayerWindow.h"
#include <thread/FFL_Thread.hpp>
#include "SDL2Module.hpp"
#include <utils/FFL_MemoryWatch.hpp>


class PlayerThread : public FFL::Thread {
public:	
	virtual bool threadLoop();

	player::FFLPlayer* mPlayer;

};

bool PlayerThread::threadLoop() {

	player::FFLPlayer* player = new player::FFLPlayer();
	mPlayer = player;
	player::FFLPlayer::createAndsetSDL2Render(player);
	player->mUrl = "d://movic//sintel.ts";	
	player::SDL2Loop(NULL, &player);
	player->release();
	return true;
}

static PlayerThread*  gThread;
player::FFLPlayer* createPlayer() {
	gThread =new  PlayerThread();
	gThread->run();	
	return gThread->mPlayer;
}

player::FFLPlayer* getPlayer() {
	return gThread->mPlayer;
}