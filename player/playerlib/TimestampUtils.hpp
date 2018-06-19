#ifndef _TIMESTAMPS_UTILS_HPP_
#define _TIMESTAMPS_UTILS_HPP_
#include <FFL.h>
#include <utils/FFL_Clock.hpp>

namespace player {	
	//
	//  ʱ�����λת��usֵ
	//
	int64_t timestampToUs(int64_t timestamp,const FFL::TimeBase& tb);
	//
	//  ʱ�����λת��usֵ
	//
	int64_t timestampToUs(int64_t timestamp, const FFL::TimeBase& tb,FFL::Clock* clock);
	//
	//  usתʱ�����λ
	//
	int64_t usToTimestamp(int64_t us, const FFL::TimeBase& tb);
}

#endif
