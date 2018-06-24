
#ifndef _SYNC_CLOCK_HPP_
#define _SYNC_CLOCK_HPP_

#include <FFL.h>
#include <utils/FFL_Clock.hpp>

namespace player {
	class SyncClock {
	public:
		SyncClock();
		~SyncClock();
		//
		//  重置一下同步时钟
		//
		void reset();
		//
		// 更新时钟
		//
		void updateClock(int64_t us);
		void updateClock(int64_t timestamp, const FFL::TimeBase& tb);
		//
		// 获取当前时钟
		//
		int64_t getClock();
	private:
		//
		//  外部updateClock更新的时钟us
		//
		int64_t mClock;
		//
		//  可以理解为世界时钟，这个使用的FFL_getNowus()获取时间
		//
		int64_t mWorldClock;
	};

	//
	//  计算2个时钟的差值 us
	//   c1-c2;
	//
	int64_t diffClock(SyncClock* c1, SyncClock* c2);
}

#endif
