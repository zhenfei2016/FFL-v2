/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  TimestampExtrapolator
*  Created by zhufeifei(34008081@qq.com) on 2018/05/26
*  ʱ�������
*/
#include "TimestampExtrapolator.hpp"

namespace player {
	TimestampExtrapolator::TimestampExtrapolator(FFL::sp<FFL::Clock> clock) :
		mOriginLocalTimeUs(0),
		mOriginTimestampUs(0),
		mClock(clock){
		mLastTimestamp = 0;
		mLastTimestampLocalTimeUs = 0;
	}

	TimestampExtrapolator::~TimestampExtrapolator() {
	}
	//
	// ����
	//
	void TimestampExtrapolator::reset() {
		mOriginLocalTimeUs = 0;
		mOriginTimestampUs = 0;
		mLastTimestamp = 0;
		mLastTimestampLocalTimeUs = 0;
	}
	//
	//  ����һ��ʱ�ӣ���ǰʱ�����Ӧ��ǰ��ʱ���
	//
	void TimestampExtrapolator::update(int64_t timestamp, FFL::TimeBase units) {		
		mOriginTimestampUs = timestampToUs(timestamp, units);
		mOriginLocalTimeUs = mClock->nowUs();
	}
	//
	//  ��ȡ��ǰ��ʱ��
	//
	int64_t TimestampExtrapolator::getNowUs() {
		return mClock->nowUs();
	}
	//
	// ��ȡ���ʱ��������ڵ����ʱ��
	//
	int64_t TimestampExtrapolator::getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase units) {
		int64_t d1 = 0;
		int64_t d2 = 0;
		int64_t now = 0;
		if (mOriginLocalTimeUs != 0 && mOriginTimestampUs != 0) {
			int64_t t2 = timestampToUs(timestamp, units);
			int64_t t1 = now= mClock->nowUs();
			//
			//  ������ԵĲ�ֵ
			//	
			d1 = t1 - mOriginLocalTimeUs;
			d2 = t2 - mOriginTimestampUs;
			if (d2 < 0 || d1 < 0) {
				d2 += 0;
				d1 += 0;
			}
		}

		FFL_LOG_CRIT("Extrapolator::getDelay ts=%" lld64 " d2=%" lld64 " d1=%" lld64 " delay=%" lld64,
			timestamp,
			d2,
			d1,
			d2-d1);

		int64_t delay = d2 - d1;
		int64_t timestampLocalTimeUs= now + delay;
		if (abs(timestampLocalTimeUs - mLastTimestampLocalTimeUs) > 2 * 1000 * 1000) {
			//
			//   2���������2s�����ˣ�����е��쳣��
			//
			delay=d1 = d2 = 0;
			reset();
			update(timestamp, units);
			FFL_LOG_CRIT("reset TimestampExtrapolator");
		}

		if (mLastTimestampLocalTimeUs != 0 && mLastTimestamp != 0) {
			//
			// ��ʱ���������ӳٱȽ�һ�£����ж��ٵ�ƫ��
			//
			int64_t diff = getDelayUsTimestamp(timestamp, units);
			diff -= (now - mLastTimestampLocalTimeUs);
			if (abs(diff - delay) < 1000 * 15) {				
				delay = diff;
			}else {
				//
				//  �����Ҫ��ô��������Ƶ�����ʱ�ӵ�ʱ��Ƚϳ���
				//
				FFL_LOG_CRIT("TimestampExtrapolator diff> 15ms =%" lld64 , diff - delay);
				//delay = diff;
			}
		}
		
		mLastTimestamp = timestamp;
		mLastTimestampLocalTimeUs = now + delay;
		return delay;
	}

	int64_t TimestampExtrapolator::timestampToUs(int64_t timestamp, FFL::TimeBase units) {
		if (units.mDen == 0) {
			return timestamp;
		}

		//
		//  ʱ���һ���̶ȶ�Ӧ���ٺ���
		//
		double dus = ((double)(1000 * 1000 * units.mNum)) / units.mDen;
		return (int64_t)(timestamp* dus);
	}
	//
	//  �������һ�ε�ʱ�����������ӳ�ֵ
	//
	int64_t TimestampExtrapolator::getDelayUsTimestamp(int64_t timestamp, FFL::TimeBase units) {
		if (mLastTimestamp == 0 || mLastTimestampLocalTimeUs == 0) {
			return 0;
		}

		if (timestamp == mLastTimestamp) {
			return 0;
		}

		int64_t diffUs=timestampToUs(timestamp - mLastTimestamp, units);

		return diffUs;
	}
		

}