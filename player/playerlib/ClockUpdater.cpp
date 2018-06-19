#include "ClockUpdater.hpp"

namespace player {
	ClockUpdater::ClockUpdater() {
	}
	ClockUpdater::~ClockUpdater() {
	}
	//
	//  设置这个是更新哪一个时钟的
	//
	void ClockUpdater::setClock(FFL::sp<FFL::Clock> clock) {
		mClock = clock;
	}
	//
	//  更新时钟
	//
	void ClockUpdater::updateClcok(int64_t tm, FFL::TimeBase* tb, void* uesrdata) {
		
	}
	//
	//  更新时钟
	//
	void ClockUpdater::updateClcok(int64_t tm, int32_t streamId, void* uesrdata) {

	}
}
