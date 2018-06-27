#include "NativeFFLPlayer.hpp"

namespace android{
	NativeFFLPlayer::NativeFFLPlayer(){
		
	}
	//
	// 设置渲染到的目标
	//
	status_t NativeFFLPlayer::setSurface(jobject surface){
		return FFL_OK;
	}
	//
	// 设置渲染窗口的大小，只有窗口存在的情况下才可以设置大小，否则返回失败
	//
	status_t NativeFFLPlayer::setSurfaceSize(int32_t widht,int32_t height){
		return FFL_OK;
	}
	//
	//  播放，暂停，停止
	//
	status_t NativeFFLPlayer::prepare(){
		return FFL_OK;
	}
	status_t NativeFFLPlayer::start(){
		return FFL_OK;
	}
	//
	//  pause:1 进行暂停， 0：恢复
	//
	status_t NativeFFLPlayer::pause(int32_t pause){
		return FFL_OK;
	}
	status_t NativeFFLPlayer::stop(){
		return FFL_OK;
	}
	//
	//  定位到指定us处
	//  获取当前的播放位置us
	//  获取总的播放时长us
	//
	status_t NativeFFLPlayer::seekTo(int64_t us){
		return FFL_OK;
	}
	int64_t  NativeFFLPlayer::getCurrentPosition(){
		return FFL_OK;
	}
	int64_t  NativeFFLPlayer::getDuration(){
		return FFL_OK;
	}
	//
	//  获取，设置播放速度，正常速度=100
	//
	uint32_t NativeFFLPlayer::getSpeed(){
		return FFL_OK;
	}
	void NativeFFLPlayer::setSpeed(uint32_t speed){ }
	//
	// 获取，设置音量
	//  0-255
	//
	void NativeFFLPlayer::setVolume(int32_t volume){ }
	void NativeFFLPlayer::getVolume(int32_t& volume){ }
	//
	// 获取，设置循环播放次数
	// 如果<0 : 一直循环播放
	//     =0 : 播放一次
	//     >0 : 播放num+1次
	//
	void NativeFFLPlayer::setLoop(int32_t num){ }
	int32_t NativeFFLPlayer::getLoop(){
		return FFL_OK;
	}
	//
	// 获取，设置一些特定的选项
	//
	void NativeFFLPlayer::setOptionString(const char* name, const char* value){ }
	void NativeFFLPlayer::getOptionString(const char* name, char* buf, uint32_t bufSize, const char* defaultVal){ }
	//
	// 获取，设置一些特定的选项
	//
	void NativeFFLPlayer::setOptionInt64(const char* name, int64_t value){ }
	void NativeFFLPlayer::getOptionInt64(const char* name, int64_t* value, int64_t defaultVal){ }
}