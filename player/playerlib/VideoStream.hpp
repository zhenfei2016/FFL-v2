#ifndef _VIDEO_STREAM_HPP_
#define _VIDEO_STREAM_HPP_

#include "Stream.hpp"
namespace player {
	class VideoStream : public Stream {
	public:
		VideoStream();
		virtual ~VideoStream();
		//
		// ��ȡ���ȣ��߶�
		//
		virtual void getSize(uint32_t& width, uint32_t& height)=0;
	};
}
#endif