#ifndef _AUDIO_STREAM_HPP_
#define _AUDIO_STREAM_HPP_

#include "Stream.hpp"
#include "AudioFormat.hpp"

namespace player {
	class AudioStream : public Stream {
	public:
		AudioStream();		
		virtual ~AudioStream();	
		//
		// 获取这个流的格式
		//
		virtual void getFormat(AudioFormat& fmt);
	};
}

#endif