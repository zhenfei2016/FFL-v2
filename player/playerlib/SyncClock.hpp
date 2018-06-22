
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
		//  ����һ��ͬ��ʱ��
		//
		void reset();
		//
		// ����ʱ��
		//
		void updateClock(int64_t us);
		void updateClock(int64_t timestamp, const FFL::TimeBase& tb);
		//
		// ��ȡ��ǰʱ��
		//
		int64_t getClock();
	private:
		//
		//  �ⲿupdateClock���µ�ʱ��us
		//
		int64_t mClock;
		//
		//  �������Ϊ����ʱ�ӣ����ʹ�õ�FFL_getNowus()��ȡʱ��
		//
		int64_t mWorldClock;
	};

	//
	//  ����2��ʱ�ӵĲ�ֵ us
	//   c1-c2;
	//
	int64_t diffClock(SyncClock* c1, SyncClock* c2);
}

#endif
