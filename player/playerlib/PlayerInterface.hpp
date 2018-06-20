#include <FFL.h>

namespace player {
	class IPlayer;
	class IPlayerListener;

	class IPlayer {
	public:		
		//
		//  设置播放url
		//
		virtual status_t setUrl(const char *url) = 0;
		//
		// 设置渲染到的目标
		//
		virtual status_t setSurface(void* surface) = 0;
		//
		//  播放，暂停，停止
		//
		virtual status_t prepare() = 0;
		virtual status_t start() = 0;
		virtual status_t pause() = 0;
		virtual status_t stop() = 0;
		//
		//  定位到指定us处
		//  获取当前的播放位置us
		//  获取总的播放时长us
		//
		virtual status_t seekTo(int64_t us) = 0;
		virtual int64_t  getCurrentPosition() = 0;
		virtual int64_t  getDuration() = 0;
		//
		//  获取，设置播放速度，正常速度=100
		//
		virtual uint32_t getSpeed() = 0;
		virtual void setSpeed(uint32_t speed) = 0;
		//
		// 获取，设置音量
		//
		virtual void setVolume(float left, float right) = 0;
		virtual void getVolume(float* left, float* right) = 0;
		//
		// 获取，设置循环播放次数
		// 如果<0 : 一直循环播放
		//     =0 : 播放一次
		//     >0 : 播放num+1次
		virtual void setLoop(int32_t num) = 0;
		virtual int32_t getLoop() = 0;
		//
		// 获取，设置一些特定的选项
		//
		virtual void setOptionString(const char* name, const char* value) = 0;
		virtual void getOptionString(const char* name, char* buf,uint32_t bufSize,const char* defaultVal) = 0;
		//
		// 获取，设置一些特定的选项
		//
		virtual void setOptionInt64(const char* name, int64_t value) = 0;
		virtual void getOptionInt64(const char* name, int64_t* value, int64_t defaultVal)=0;
		//
		//  设置播放器一系列事件的回调
		//
		virtual void setListener(IPlayerListener* listener) = 0;
	};

	class IPlayerListener {
	public:
		virtual void onEvent(int32_t eventId) = 0;
	};
}
