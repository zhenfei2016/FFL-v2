#ifndef _VIDEO_SCALE_HPP_
#define _VIDEO_SCALE_HPP_

#include <utils/FFL_Clock.hpp>
//
//  用于更新时钟的
//
namespace player {
	class ClockUpdater {
	public:
		ClockUpdater();
		virtual ~ClockUpdater();
	public:
		//
		//  设置这个是更新哪一个时钟的
		//
		void setClock(FFL::sp<FFL::Clock> clock);
		//
		//  更新时钟
		//
		virtual void updateClcok(int64_t tm,FFL::TimeBase* tb,void* uesrdata);

		//
		//  更新时钟
		//
		virtual void updateClcok(int64_t tm, int32_t streamId, void* uesrdata);

	private:
		FFL::sp<FFL::Clock> mClock;
	};
}

#endif