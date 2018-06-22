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
		TimestampExtrapolator();
		~TimestampExtrapolator();
		//
		// ����
		//
		void reset();
		//
		//  ����ʱ�ӵ��ٶ�
		//
		void setSpeed(uint32_t speed);
		uint32_t getSpeed() const;
		////
		////  �������������������ʱ��㣬
		////
		//void update(int64_t timestamp, const FFL::TimeBase& tb);
		////
		//// ��ȡ���ʱ�����Ե�ǰ��Ҫ�ӳٵ�ʱ��us
		////
		//int64_t getDelay(int64_t timestamp,const FFL::TimeBase& tb);
		int64_t getDelayAndUpdate(int64_t timestamp, const FFL::TimeBase& tb);
	private:	
		struct Tick{
			Tick() {
				mTimestampClock = mWorldClock = 0;
			}

			bool isValid() { 
				return mTimestampClock != 0 && mWorldClock != 0;
			}
			//
			//  ʱ�����Ӧ��ʱ��㣬ͨ��ʱ���ֱ�Ӽ����
			//
			int64_t mTimestampClock;
			//
			//  ��ǰmClock��Ӧ��ʱ���
			//
			int64_t mWorldClock;
			int64_t mDelayUs;
		};
		//
		//  �����ʱ�ӵδ���update���и���
		//
		Tick mRecentTick;	
		uint32_t mSpeed;
	};
}

#endif
