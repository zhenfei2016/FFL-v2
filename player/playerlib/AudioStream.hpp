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
		// ��ȡ������ĸ�ʽ
		//
		virtual void getFormat(AudioFormat& fmt);
	};
}

#endif