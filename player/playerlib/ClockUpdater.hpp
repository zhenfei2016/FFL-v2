#ifndef _VIDEO_SCALE_HPP_
#define _VIDEO_SCALE_HPP_

#include <utils/FFL_Clock.hpp>
//
//  ���ڸ���ʱ�ӵ�
//
namespace player {
	class ClockUpdater {
	public:
		ClockUpdater();
		virtual ~ClockUpdater();
	public:
		//
		//  ��������Ǹ�����һ��ʱ�ӵ�
		//
		void setClock(FFL::sp<FFL::Clock> clock);
		//
		//  ����ʱ��
		//
		virtual void updateClcok(int64_t tm,FFL::TimeBase* tb,void* uesrdata);

		//
		//  ����ʱ��
		//
		virtual void updateClcok(int64_t tm, int32_t streamId, void* uesrdata);

	private:
		FFL::sp<FFL::Clock> mClock;
	};
}

#endif