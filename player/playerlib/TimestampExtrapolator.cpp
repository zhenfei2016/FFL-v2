/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  TimestampExtrapolator
*  Created by zhufeifei(34008081@qq.com) on 2018/05/26
*  时间戳推算
*/
#include "TimestampExtrapolator.hpp"
#include "PlayerConstant.hpp"
#include "TimestampUtils.hpp"

namespace player {
	TimestampExtrapolator::TimestampExtrapolator(FFL::sp<FFL::Clock> clock) :
		mClock(clock){	
	}

	TimestampExtrapolator::~TimestampExtrapolator() {
	}
	//
	// 重置
	//
	void TimestampExtrapolator::reset() {
		mRecentTick.mClockTime = 0;
		mRecentTick.mTimestampTime = 0;
	}
	//
	//  重置一下时钟，当前时间戳对应当前的时间点
	//
	void TimestampExtrapolator::update(int64_t timestamp,const FFL::TimeBase& tb) {
		int64_t updateTimestampUs = timestampToUs(timestamp, tb);

		if (mRecentTick.mTimestampTime != updateTimestampUs) {			
			int64_t now = getNowUs();
			FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "Extrapolator: update timestampUs=%" lld64 " localUs=%" lld64 " difftm=%" lld64 " difflocal = %" lld64,								
				updateTimestampUs,
				now,				
				updateTimestampUs- mRecentTick.mTimestampTime,
				now - mRecentTick.mClockTime);

			mRecentTick.mTimestampTime = updateTimestampUs;
			mRecentTick.mClockTime = now;

		}
		if (timestamp < 1024) {
			int i = 0;
			i++;
		}
	}
	//
	//  获取当前的时间
	//
	int64_t TimestampExtrapolator::getNowUs() {
		return mClock->nowUs();
	}
	//
	// 获取这个时间戳到现在的相对时间
	//
	int64_t TimestampExtrapolator::getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase tb) {
		if (!mRecentTick.isValid()) {
			return 0;
		}

		int64_t timestampUs = timestampToUs(timestamp, tb);
		int64_t clockUs = mClock->nowUs();

		//
		// 根据时间戳计算相对上一个时间戳，要等待多长时间
		//
		int64_t deltaTimestamp= (timestampUs - mRecentTick.mTimestampTime);
		if (mClock->speed() != 100) {
			deltaTimestamp = (deltaTimestamp * 100) / mClock->speed();
		}

		//
		//  本地clock已经过去了多长时间了
		//
		int64_t deltaClock = clockUs - mRecentTick.mClockTime;
		if (deltaTimestamp < 0 || deltaClock < 0) {
            FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP,"Extrapolator: get timestampUs=%" lld64 " deltaTimestamp<0 || deltaClock<0  deltaTimestamp=%" lld64 " deltaClock=%" lld64,
				timestampUs, deltaTimestamp, deltaClock);
		}

		int64_t delay = deltaTimestamp - deltaClock;

		FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "Extrapolator: get timestampUs=% " lld64 " deltaTimestamp=%" lld64 " deltaClock=%" lld64 " delay=%" lld64,
			timestampUs, deltaTimestamp, deltaClock,delay);
		return delay;
	}

	

}
