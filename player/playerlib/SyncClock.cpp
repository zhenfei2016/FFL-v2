#include "SyncClock.hpp"
#include "TimestampUtils.hpp"

namespace player {
	SyncClock::SyncClock() {
		reset();
	}
	SyncClock::~SyncClock() {

	}
	//
	//  ����һ��ͬ��ʱ��
	//
	void SyncClock::reset() {
		mClock = 0;
		mWorldClock = 0;
	}
	//
	// ����ʱ��
	//
	void SyncClock::updateClock(int64_t us) {
		mClock = us;
		mWorldClock = FFL_getNowUs();
	}
	void SyncClock::updateClock(int64_t timestamp, const FFL::TimeBase& tb) {
		updateClock(timestampToUs(timestamp, tb));
	}
	//
	// ��ȡ��ǰʱ��
	//
	int64_t SyncClock::getClock() {
		if (mClock == 0 || mWorldClock == 0) {
			return 0;
		}
		return mClock + FFL_getNowUs() - mWorldClock;
	}

	//
	//  ����2��ʱ�ӵĲ�ֵ us
	//
	int64_t diffClock(SyncClock* c1, SyncClock* c2) {
		if (c1 == c2) {
			return 0;
		}
		return c1->getClock() - c2->getClock();
	}
}