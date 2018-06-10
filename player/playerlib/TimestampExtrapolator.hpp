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
		void update(int64_t timestamp, FFL::TimeBase units);
		//
		//  ��ȡ��ǰ��ʱ��
		//
		int64_t getNowUs();
		//
		// ��ȡ���ʱ�����Ե�ǰ��Ҫ�ӳٵ�ʱ��us
		//
		int64_t getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase units);
	private:
		//
		//  ʱ�����λת��usֵ
		//
		int64_t timestampToUs(int64_t timestamp, FFL::TimeBase units);
		//
		//  �������һ�ε�ʱ�����������ӳ�ֵ
		//
		int64_t getDelayUsTimestamp(int64_t timestamp, FFL::TimeBase units);
	private:
		//
		//  Դ���ʱ��㣬����ʱ�������������ƫ��ֵ  ������ϵͳ��ʱ����
		//
		int64_t mOriginLocalTimeUs;
		//
		//  �ⲿ�����ʱ����Ŀ�ʼʱ��㣬�ⲿ��ʱ����
		//
		int64_t mOriginTimestampUs;
		//
		//  ���һ�λ�ȡdelay��ʱ������ͼ����clockʱ���
		//
		int64_t mLastTimestamp;
		int64_t mLastTimestampLocalTimeUs;
	public:
		FFL::sp<FFL::Clock> mClock;
	};
}

#endif
