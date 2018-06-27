#ifndef _ANDROID_AUDIO_DEVICE_HPP_
#define _ANDROID_AUDIO_DEVICE_HPP_
#include "AudioDevice.hpp"
#include <FFL.h>
namespace android {
	class JavaAudioTrack;
	class AndroidAudioDevice : public player::AudioDevice {
	public:
		AndroidAudioDevice();
		virtual ~AndroidAudioDevice();

	public:
		//
		//  获取支持的格式
		//  wanted: 如果为nUll则返回所有支持的格式
		//           非null 返回跟他匹配的 
		//  fmtList: 返回支持的格式list
		//
		virtual void getSupportFormat(const player::AudioFormat* wanted, FFL::List<player::AudioFormat>& fmtList) ;
		virtual bool isSupportFormat(const player::AudioFormat* wanted) ;
		//
		// 设置缓冲多长时间的数据
		//
		virtual void setCacheUs(int64_t us) ;
		//
		//  打开关闭音频设备
		//
		virtual bool open(const player::AudioFormat& wanted, int32_t sampleNum, player::AudioFormat& obtained) ;
		virtual void close() ;
		//
		//获取当前打开的格式，如果未打开则返回null
		//
		virtual player::AudioFormat* getOpenFormat() ;
		//
		//  写一帧数据
		//
		virtual bool writeFrame(player::AudioSample* samples) ;
		//
		//  获取播放设备硬件的延迟
		//
		virtual int64_t getDeviceDelayUs() ;
		//
		// 获取缓冲的数据量，还没有播放
		//
		virtual int64_t getCacheBytes() ;
		virtual int64_t getCacheDelay(FFL::TimeBase& base) ;
		//
		//  清空缓冲的数据
		//
		virtual int64_t clearCache() ;
		//
		// 获取播放中的音频的pts
		//
		virtual int64_t getRenderingPts() ;

	protected:
		JavaAudioTrack* mAudioTrack;
	};
}
#endif
