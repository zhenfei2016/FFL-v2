/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  TimestampExtrapolator   
*  Created by zhufeifei(34008081@qq.com) on 2018/05/26 
*
*  ����Ƶͬ����ʱ������������������timestamp��Ҫ�ӳٶ೤ʱ�俪ʼ����
*  
*  ��ʱ����Ҫһֱ update();
*  ��Ҫ�����ӳ�ʱ����  getDelayUsRelativeNow(),�������������Ҫ�ӳٶ೤ʱ��
*/
#ifndef _TIMESTAMPEXTRAPOLATOR_HPP_
#define _TIMESTAMPEXTRAPOLATOR_HPP_

#include <FFL.h>
#include <utils/FFL_Clock.hpp>
#include "Stream.hpp"

namespace player {
	class TimestampExtrapolator {
	public:
		//
		//  clock: ������һ��ʱ�ӣ�����ʱ�������
		//  ���Ըı����clock���ٶȣ���������������ӳ�ֵ����Ӧ�ĸı���
		// 
		TimestampExtrapolator(FFL::sp<FFL::Clock> clock);
		~TimestampExtrapolator();
		//
		// ����
		//
		void reset();
		//
		//  �������������������ʱ��㣬��ǰʱ�����Ӧ��ǰ��ʱ���
		//  ��Ӧ����һ����Ҫͬ��������������ʱ�ӽ��е�
		//
		void update(int64_t timestamp, const FFL::TimeBase& units);
		//
		//  ��ȡ��ǰ��ʱ��
		//
		int64_t getNowUs();
		//
		// ��ȡ���ʱ�����Ե�ǰ��Ҫ�ӳٵ�ʱ��us
		//
		int64_t getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase units);	
	private:	
		struct Tick{
			Tick() {
				mTimestampTime = mClockTime = 0;
			}

			bool isValid() { 
				return mTimestampTime != 0 && mClockTime != 0;
			}
			//
			//  ʱ�����Ӧ��ʱ��㣬ͨ��ʱ���ֱ�Ӽ����
			//
			int64_t mTimestampTime;
			//
			//  ��ǰmClock��Ӧ��ʱ���
			//
			int64_t mClockTime;
		};
		//
		//  �����ʱ�ӵδ���update���и���
		//
		Tick mRecentTick;	
		FFL::sp<FFL::Clock> mClock;
	};
}

#endif
