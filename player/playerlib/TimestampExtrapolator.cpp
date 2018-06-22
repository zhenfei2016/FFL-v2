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
	TimestampExtrapolator::TimestampExtrapolator(){			
		reset();
	}

	TimestampExtrapolator::~TimestampExtrapolator() {
	}
	//
	// 重置
	//
	void TimestampExtrapolator::reset() {
		mRecentTick.mWorldClock = 0;
		mRecentTick.mTimestampClock = 0;
		mRecentTick.mDelayUs = 0;
		setSpeed(100);
	}
	//
	//  设置时钟的速度
	//
	void TimestampExtrapolator::setSpeed(uint32_t speed) {
		mSpeed = speed==0?1:speed;
	}
	uint32_t TimestampExtrapolator::getSpeed() const {
		return mSpeed;
	}

	////
	////  重置一下时钟，当前时间戳对应当前的时间点
	////
	//void TimestampExtrapolator::update(int64_t timestamp,const FFL::TimeBase& tb) {
	//	int64_t updateTimestampUs = timestampToUs(timestamp, tb);

	//	if (mRecentTick.mTimestampClock != updateTimestampUs) {			
	//		int64_t now = FFL_getNowUs();			
	//		mRecentTick.mTimestampClock = updateTimestampUs;
	//		mRecentTick.mWorldClock = now;
	//	}
	//	if (timestamp < 1024) {
	//		int i = 0;
	//		i++;
	//	}
	//}
	////
	//// 获取这个时间戳到现在的相对时间
	////
	//int64_t TimestampExtrapolator::getDelay(int64_t timestamp, const FFL::TimeBase& tb) {
	//	if (!mRecentTick.isValid()) {
	//		return 0;
	//	}

	//	int64_t timestampUs = timestampToUs(timestamp, tb);
	//	int64_t clockUs = FFL_getNowUs();

	//	int64_t delay = timestampUs - mRecentTick.mTimestampClock;
	//	delay -=(clockUs - mRecentTick.mWorldClock);
	//	return delay;
	//}

	//
	// 获取这个时间戳到现在的相对时间
	//
	int64_t TimestampExtrapolator::getDelayAndUpdate(int64_t timestamp, const FFL::TimeBase& tb) {
		int64_t timestampUs = timestampToUs(timestamp, tb);
		int64_t worldUs = FFL_getNowUs();

		int64_t delay = 0;
		if (mRecentTick.isValid()) {			
			delay = timestampUs - mRecentTick.mTimestampClock;
			//
			// 因为整体速度改变了，world时钟相应的也需要进行调整
			//
			//
			double distance= ((double)((worldUs - mRecentTick.mWorldClock) * mSpeed)) / 100  - mRecentTick.mDelayUs;
			delay -= (int64_t)distance;
		}

		mRecentTick.mWorldClock = worldUs;
		mRecentTick.mTimestampClock = timestampUs;
		mRecentTick.mDelayUs = delay;

		FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "TimestampExtrapolator::getDelayAndUpdate(%p) timestampUs=%" lld64 " worldClock=%" lld64 " delay=%" lld64,
			this,
			mRecentTick.mTimestampClock,
			mRecentTick.mWorldClock,
			delay);	
		
		return delay;
	}

}
